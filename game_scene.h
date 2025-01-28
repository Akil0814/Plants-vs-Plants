#pragma once

#include "scene_manager.h"
#include "scene.h"
#include "util.h"
#include"platform.h"
#include"player.h"
#include"bullet.h"
#include"status_bar.h"

#include <vector>

extern Player* player_1;
extern Player* player_2;

extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;

extern Camera main_camera;
extern std::vector<Platform> platform_list;

extern IMAGE* img_player_1_avatar;
extern IMAGE* img_player_2_avatar;

extern SceneManager scene_manager;

class GameScene :public Scene//游戏场景派生类
{
public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter()
	{
		status_bar_1P.set_avatar(img_player_1_avatar);//设置玩家头像
		status_bar_2P.set_avatar(img_player_2_avatar);

		status_bar_1P.set_position(235, 625);//设置角色头像位置
		status_bar_2P.set_position(675, 625);

		player_1->set_position(200, 50);//初始化角色位置
		player_2->set_position(975, 50);

		pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;//居中渲染天空背景图

		pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;//居中渲染山脉背景图

		platform_list.resize(4);//创建长度为4的平台列表


		Platform& large_platform = platform_list[0];//创建平台对象
		large_platform.img = &img_platform_large;//初始化图像指针
		large_platform.render_positon.x = 122;//初始化平台渲染位置
		large_platform.render_positon.y = 455;//初始化平台渲染位置
		large_platform.shape.left = (float)large_platform.render_positon.x + 30;//初始化平台物理位置
		large_platform.shape.right = (float)large_platform.render_positon.x + img_platform_large.getwidth() - 30;//初始化平台物理位置
		large_platform.shape.y = (float)large_platform.render_positon.y + 60;//初始化平台物理高度水平高度

		Platform& small_platform_1 = platform_list[1];//创建平台对象
		small_platform_1.img = &img_platform_small;
		small_platform_1.render_positon.x = 175;
		small_platform_1.render_positon.y = 360;
		small_platform_1.shape.left = (float)small_platform_1.render_positon.x + 40;
		small_platform_1.shape.right = (float)small_platform_1.render_positon.x + img_platform_small.getwidth() - 40;
		small_platform_1.shape.y = (float)small_platform_1.render_positon.y + img_platform_small.getheight() / 2;

		Platform& small_platform_2 = platform_list[2];//创建平台对象
		small_platform_2.img = &img_platform_small;
		small_platform_2.render_positon.x = 855;
		small_platform_2.render_positon.y = 360;
		small_platform_2.shape.left = (float)small_platform_2.render_positon.x + 40;
		small_platform_2.shape.right = (float)small_platform_2.render_positon.x + img_platform_small.getwidth() - 40;
		small_platform_2.shape.y = (float)small_platform_2.render_positon.y + img_platform_small.getheight() / 2;

		Platform& small_platform_3 = platform_list[3];//创建平台对象
		small_platform_3.img = &img_platform_small;
		small_platform_3.render_positon.x = 515;
		small_platform_3.render_positon.y = 225;
		small_platform_3.shape.left = (float)small_platform_3.render_positon.x + 40;
		small_platform_3.shape.right = (float)small_platform_3.render_positon.x + img_platform_small.getwidth() - 40;
		small_platform_3.shape.y = (float)small_platform_3.render_positon.y + img_platform_small.getheight() / 2;

		//mciSendString(_T("play bgm_game repeat from 0"), NULL, 0, NULL);//导入Bgm


	}//场景进入时初始化逻辑

	void on_update(int delta)
	{
		player_1->on_update(delta);
		player_2->on_update(delta);

		main_camera.on_update(delta);//更新主摄像机

		bullet_list.erase(std::remove_if(
			bullet_list.begin(), bullet_list.end(),
			[](const Bullet* bullet)
			{
				bool deletable = bullet->check_can_remove();//检测是否可以被删除
				if (deletable) delete bullet;//删除指向的对象
				return deletable;//返回指针//让更上层决定是否删除指针本身
			}),
			bullet_list.end());

		for (Bullet* bullet : bullet_list)//遍历子弹对象
			bullet->on_update(delta);//依次调用更新方法
	}//处理数据

	void on_draw(const Camera& camera)
	{
		putimage_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
		putimage_alpha(camera, pos_img_hills.x, pos_img_hills.y, &img_hills);

		for (const Platform& platform : platform_list)//遍历每个平台对象
			platform.on_draw(camera);//依次调用 on draw 方法

		if (is_debug)
		{
			settextcolor(RGB(255, 0, 0));
			outtextxy(15, 15, _T("调试模式"));
		}

		player_1->on_draw(camera);//绘制玩家图像
		player_2->on_draw(camera);

		for (Bullet* bullet : bullet_list)
			bullet->on_draw(camera);

		status_bar_1P.set_hp(player_1->get_hp());//设置玩家hp mp
		status_bar_1P.set_mp(player_1->get_mp());
		status_bar_2P.set_hp(player_2->get_hp());
		status_bar_2P.set_mp(player_2->get_mp());

		status_bar_1P.on_draw();
		status_bar_2P.on_draw();

	}//渲染绘图

	void on_input(const ExMessage& msg)
	{
		player_1->on_input(msg);//将操作信息传递给玩家
		player_2->on_input(msg);

		switch (msg.message)//按下Q开启调试模式
		{
		case WM_KEYUP:
			if (msg.vkcode == 0x51)//Q
				is_debug = !is_debug;
			break;
		default:
			break;
		}
	}//处理玩家输入

	void on_exit()
	{
	}//场景退出时的卸载逻辑

private:
	POINT pos_img_sky = { 0 };//天空坐标位置
	POINT pos_img_hills = { 0 };//山脉坐标位置

	StatusBar status_bar_1P;//玩家1状态条
	StatusBar status_bar_2P;//玩家2状态条
};