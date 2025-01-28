#pragma once


#include "scene.h"
#include "player_id.h"
#include "peashooter_player.h"
#include "sunflower_player.h"

extern Player* player_1;//声明指针
extern Player* player_2;

extern IMAGE img_VS;
extern IMAGE img_1P;
extern IMAGE img_2P;
extern IMAGE img_1P_desc;
extern IMAGE img_2P_desc;
extern IMAGE img_gravestone_left;
extern IMAGE img_gravestone_right;
extern IMAGE img_selector_tip;
extern IMAGE img_selector_background;
extern IMAGE img_1P_selector_btn_idle_left;
extern IMAGE img_1P_selector_btn_idle_right;
extern IMAGE img_1P_selector_btn_down_left;
extern IMAGE img_1P_selector_btn_down_right;
extern IMAGE img_2P_selector_btn_idle_left;
extern IMAGE img_2P_selector_btn_idle_right;
extern IMAGE img_2P_selector_btn_down_left;
extern IMAGE img_2P_selector_btn_down_right;
extern IMAGE img_peashooter_selector_background_left;
extern IMAGE img_peashooter_selector_background_right;
extern IMAGE img_sunflower_selector_background_left;
extern IMAGE img_sunflower_selector_background_right;

extern Atlas atlas_peashooter_idle_right;
extern Atlas atlas_sunflower_idle_right;

extern IMAGE img_avatar_peashooter;
extern IMAGE img_avatar_sunflower;

extern IMAGE* img_player_1_avatar;
extern IMAGE* img_player_2_avatar;

extern SceneManager scene_manager;

class SelectorScene :public Scene//玩家角色选择界面
{
public:
	SelectorScene() = default;
	~SelectorScene() = default;

	void on_enter()
	{
		static const int OFFSET_X = 50;

		animation_peashooter.set_atlas(&atlas_peashooter_idle_right);//设置角色图集
		animation_sunflower.set_atlas(&atlas_sunflower_idle_right);//设置角色图集
		animation_peashooter.set_interval(100);//设置帧间隔
		animation_sunflower.set_interval(100);//设置帧间隔

		pos_img_VS.x = (getwidth() - img_VS.getwidth()) / 2;//计算游戏界面素材位置
		pos_img_VS.y = (getheight() - img_VS.getheight()) / 2;
		pos_img_tip.x = (getwidth() - img_selector_tip.getwidth()) / 2;
		pos_img_tip.y = getheight() - 125;
		pos_img_1P.x = (getwidth() / 2 - img_1P.getwidth()) / 2 - OFFSET_X;
		pos_img_1P.y = 35;
		pos_img_2P.x = getwidth() / 2 + (getwidth() / 2 - img_2P.getwidth()) / 2 + OFFSET_X;
		pos_img_2P.y = pos_img_1P.y;
		pos_img_1P_desc.x = (getwidth() / 2 - img_1P_desc.getwidth()) / 2 - OFFSET_X;
		pos_img_1P_desc.y = getheight() - 150;
		pos_img_2P_desc.x = getwidth() / 2 + (getwidth() / 2 - img_2P_desc.getwidth()) / 2 + OFFSET_X;
		pos_img_2P_desc.y = pos_img_1P_desc.y;
		pos_img_1P_gravestone.x = (getwidth() / 2 - img_gravestone_right.getwidth()) / 2 - OFFSET_X;
		pos_img_1P_gravestone.y = pos_img_1P.y + img_1P.getheight() + 35;
		pos_img_2P_gravestone.x = getwidth() / 2 + (getwidth() / 2 - img_gravestone_left.getwidth()) / 2 + OFFSET_X;
		pos_img_2P_gravestone.y = pos_img_1P_gravestone.y;
		pos_animation_1P.x = (getwidth() / 2 - atlas_peashooter_idle_right.get_image(0)->getwidth()) / 2 - OFFSET_X;
		pos_animation_1P.y = pos_img_1P_gravestone.y + 80;
		pos_animation_2P.x = getwidth() / 2 + (getwidth() / 2 - atlas_peashooter_idle_right.get_image(0)->getwidth()) / 2 + OFFSET_X;
		pos_animation_2P.y = pos_animation_1P.y;
		pos_img_1P_name.y = pos_animation_1P.y + 155;
		pos_img_2P_name.y = pos_img_1P_name.y;
		pos_1P_selector_btn_left.x = pos_img_1P_gravestone.x - img_1P_selector_btn_idle_left.getwidth();
		pos_1P_selector_btn_left.y = pos_img_1P_gravestone.y + (img_gravestone_right.getheight() - img_1P_selector_btn_idle_left.getheight()) / 2;
		pos_1P_selector_btn_right.x = pos_img_1P_gravestone.x + img_gravestone_right.getwidth();
		pos_1P_selector_btn_right.y = pos_1P_selector_btn_left.y;
		pos_2P_selector_btn_left.x = pos_img_2P_gravestone.x - img_2P_selector_btn_idle_left.getwidth();
		pos_2P_selector_btn_left.y = pos_1P_selector_btn_left.y;
		pos_2P_selector_btn_right.x = pos_img_2P_gravestone.x + img_gravestone_left.getwidth();
		pos_2P_selector_btn_right.y = pos_1P_selector_btn_left.y;
	}

	void on_update(int delta)
	{
		animation_peashooter.on_update(delta);//更新渲染的动态图片
		animation_sunflower.on_update(delta);//不更新则会停在第一帧动画帧
		selector_background_scroll_offset_x += 5;//每次渲染更新时使其移动5个像素
		if (selector_background_scroll_offset_x >= img_peashooter_selector_background_left.getwidth())//当线条距离到达剪影图片宽度时
			selector_background_scroll_offset_x = 0;//重置线条数值为0
	}

	void on_draw(const Camera& camera)
	{
		IMAGE* img_p1_selector_background = nullptr;//定义指针指向玩家背后滚动的图片对象
		IMAGE* img_p2_selector_background = nullptr;

		switch (player_type_2)//通过switch绘制角色选择的对应的角色的剪影
		{
		case PlayerType::Peashooter:
			img_p1_selector_background = &img_peashooter_selector_background_right;
			break;
		case PlayerType::Sunflower:
			img_p1_selector_background = &img_sunflower_selector_background_right;
			break;
		default:
			img_p1_selector_background = &img_peashooter_selector_background_right;
			break;
		}

		switch (player_type_1)//通过switch绘制角色选择的对应的角色的剪影
		{
		case PlayerType::Peashooter:
			img_p2_selector_background = &img_peashooter_selector_background_left;
			break;
		case PlayerType::Sunflower:
			img_p2_selector_background = &img_sunflower_selector_background_left;
			break;
		default:
			img_p2_selector_background = &img_peashooter_selector_background_left;
			break;
		}

		putimage(0, 0, &img_selector_background);

		putimage_alpha(selector_background_scroll_offset_x - img_p1_selector_background->getwidth(), 0, img_p1_selector_background);
		putimage_alpha(selector_background_scroll_offset_x, 0, img_p1_selector_background->getwidth() - selector_background_scroll_offset_x, 0, img_p1_selector_background, 0, 0);
		putimage_alpha(getwidth() - img_p2_selector_background->getwidth(), 0, img_p2_selector_background->getwidth() - selector_background_scroll_offset_x, 0, img_p2_selector_background, selector_background_scroll_offset_x, 0);
		putimage_alpha(getwidth() - selector_background_scroll_offset_x, 0, img_p2_selector_background);//绘制动态背景

		putimage_alpha(pos_img_VS.x, pos_img_VS.y, &img_VS);

		putimage_alpha(pos_img_1P.x, pos_img_1P.y, &img_1P);
		putimage_alpha(pos_img_2P.x, pos_img_2P.y, &img_2P);
		putimage_alpha(pos_img_1P_gravestone.x, pos_img_1P_gravestone.y, &img_gravestone_right);//墓碑图片绘制
		putimage_alpha(pos_img_2P_gravestone.x, pos_img_2P_gravestone.y, &img_gravestone_left);


		switch (player_type_1)//根据角色选择在对应位置渲染对应图片
		{
		case PlayerType::Peashooter:
			animation_peashooter.on_draw(camera, pos_animation_1P.x, pos_animation_1P.y);
			pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_peashooter_name)) / 2;//设置绘图位置
			outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_peashooter_name);//根据选择的角色绘制文本内容
			break;
		case PlayerType::Sunflower:
			animation_sunflower.on_draw(camera, pos_animation_1P.x, pos_animation_1P.y);
			pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_sunflower_name)) / 2;//设置绘图位置
			outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_sunflower_name);//根据选择的角色绘制文本内容
			break;
		}

		switch (player_type_2)//根据角色选择在对应位置渲染对应图片
		{
		case PlayerType::Peashooter:
			animation_peashooter.on_draw(camera, pos_animation_2P.x, pos_animation_2P.y);
			pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_left.getwidth() - textwidth(str_peashooter_name)) / 2;//设置绘图位置
			outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_peashooter_name);//根据选择的角色绘制文本内容
			break;
		case PlayerType::Sunflower:
			animation_sunflower.on_draw(camera, pos_animation_2P.x, pos_animation_2P.y);
			pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_left.getwidth() - textwidth(str_sunflower_name)) / 2;//设置绘图位置
			outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_sunflower_name);//根据选择的角色绘制文本内容
			break;
		}

		putimage_alpha(pos_img_1P_desc.x, pos_img_1P_desc.y, &img_1P_desc);
		putimage_alpha(pos_img_2P_desc.x, pos_img_2P_desc.y, &img_2P_desc);

		putimage_alpha(pos_1P_selector_btn_left.x, pos_1P_selector_btn_left.y, is_btn_1P_left_down ? &img_1P_selector_btn_down_left : &img_1P_selector_btn_idle_left);//角色选择方向键
		putimage_alpha(pos_1P_selector_btn_right.x, pos_1P_selector_btn_right.y, is_btn_1P_right_down ? &img_1P_selector_btn_down_right : &img_1P_selector_btn_idle_right);
		putimage_alpha(pos_2P_selector_btn_left.x, pos_2P_selector_btn_left.y, is_btn_2P_left_down ? &img_2P_selector_btn_down_left : &img_2P_selector_btn_idle_left);
		putimage_alpha(pos_2P_selector_btn_right.x, pos_2P_selector_btn_right.y, is_btn_2P_right_down ? &img_2P_selector_btn_down_right : &img_2P_selector_btn_idle_right);

		putimage_alpha(pos_img_tip.x, pos_img_tip.y, &img_selector_tip);//按键介绍
	}

	void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN://按键按下的消息处理
			switch (msg.vkcode)//根据按键类型进入case
			{
			case 0x41:// 'A'
				is_btn_1P_left_down = true;//设置按下为true
				break;
			case 0x44:// 'D'
				is_btn_1P_right_down = true;
				break;
			case VK_LEFT:// '←'
				is_btn_2P_left_down = true;
				break;
			case VK_RIGHT:// '→'
				is_btn_2P_right_down = true;
				break;
			}
			break;
		case WM_KEYUP://按键抬起的消息处理
			switch (msg.vkcode)//根据按键类型进入 case
			{
				
			case 0x41:// 'A'
				is_btn_1P_left_down = false;//设置按下为 false
				player_type_1 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_1 - 1) % (int)PlayerType::Invalid);
				//将Invalid也转化为 int值；加上 将玩家角色枚举变量转化为 int值并-1//确保以上两个值相加大于0；
				//将得到值 对Invalid对应的int值 进行取模；来确保最终的值不会大于或等于Invalid//将得到的枚举类型的值赋值给玩家类型
				//确保得到值在枚举类第一个到最后一个值之间 即 [peashooter,Invalid) 的区间内
				mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				break;
				
			case 0x44:// 'D'
				is_btn_1P_right_down = false;
				player_type_1 = (PlayerType)(((int)player_type_1 + 1) % (int)PlayerType::Invalid);
				mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				break;
				
			case VK_LEFT:// '←'
				is_btn_2P_left_down = false;
				player_type_2 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_2 - 1) % (int)PlayerType::Invalid);
				mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				break;
				
			case VK_RIGHT:// '→'
				is_btn_2P_right_down = false;
				player_type_2 = (PlayerType)(((int)player_type_2 + 1) % (int)PlayerType::Invalid);
				mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				break;
			case VK_RETURN://回车键
				scene_manager.switch_to(SceneManager::SceneType::Game);//切换游戏场景
				mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
				break;
			}
			break;
		}
	}

	void on_exit()
	{
		switch (player_type_1)//根据玩家选择不同的角色类型
		{
		case PlayerType::Peashooter:
			player_1 = new PeashooterPlayer();//实例化不同的角色子类对象
			img_player_1_avatar = &img_avatar_peashooter;//设置玩家头像

			break;
		case PlayerType::Sunflower:
			player_1 = new SunflowerPlayer();
			img_player_1_avatar = &img_avatar_sunflower;//设置玩家头像

			break;
		}
		player_1->set_id(PlayerID::P1);//在玩家对象创建后为ID赋值

		switch (player_type_2)//根据玩家选择不同的角色类型
		{
		case PlayerType::Peashooter:
			player_2 = new PeashooterPlayer();
			img_player_2_avatar = &img_avatar_peashooter;//设置玩家头像

			break;
		case PlayerType::Sunflower:
			player_2 = new SunflowerPlayer();
			img_player_2_avatar = &img_avatar_sunflower;//设置玩家头像

			break;
		}
		player_2->set_id(PlayerID::P2);


		mciSendString(_T("stop bgm_menu"), NULL, 0, NULL);

	}

private:
	enum class PlayerType
	{
		Peashooter = 0,
		Sunflower,
		Invalid
	};

private:
	bool is_btn_1P_left_down = false;//玩家是否按下了对应方向的按钮
	bool is_btn_1P_right_down = false;
	bool is_btn_2P_left_down = false;
	bool is_btn_2P_right_down = false;
	POINT pos_img_VS = { 0 };// VS 艺术字图片位置
	POINT pos_img_tip = { 0 };// 提示信息文本图片位置
	POINT pos_img_1P = { 0 };// 1P 文本图片位置
	POINT pos_img_2P = { 0 };// 2P 文本图片位置
	POINT pos_img_1P_desc = { 0 };// 1P 键位描述图片位置
	POINT pos_img_2P_desc = { 0 };// 2P 键位描述图片位置
	POINT pos_img_1P_name = { 0 };// 1P 角色姓名文本位置
	POINT pos_img_2P_name = { 0 };// 2P 角色姓名文本位置
	POINT pos_animation_1P = { 0 };// 1P 角色动画位置
	POINT pos_animation_2P = { 0 };// 2P 角色动画位置
	POINT pos_img_1P_gravestone = { 0 };// 1P 墓碑图片位置
	POINT pos_img_2P_gravestone = { 0 };// 2P 墓碑图片位置
	POINT pos_1P_selector_btn_left = { 0 };// 1P 向左切换按钮位置
	POINT pos_1P_selector_btn_right = { 0 };// 1P 向右切换按钮位置
	POINT pos_2P_selector_btn_left = { 0 };// 2P 向左切换按钮位置
	POINT pos_2P_selector_btn_right = { 0 };// 2P 向右切换按钮位置
	Animation animation_peashooter;//婉逗射手角色动画
	Animation animation_sunflower;//龙日葵角色动画
	PlayerType player_type_1 = PlayerType::Peashooter;//定义玩家类型
	PlayerType player_type_2 = PlayerType::Sunflower;

	LPCTSTR str_peashooter_name = _T("“豌豆射手”");//定义角色名文本字符串
	LPCTSTR str_sunflower_name = _T("“向日葵”");

	int selector_background_scroll_offset_x = 0;//背景板在水平方向上滚动的距离

private:
	void outtextxy_shaded(int x, int y, LPCTSTR str)//绘制带阴影效果的文本
	{
		settextcolor(RGB(45, 45, 45));
		outtextxy(x + 3, y + 3, str);
		settextcolor(RGB(255, 255, 255));
		outtextxy(x, y, str);
	}
};

