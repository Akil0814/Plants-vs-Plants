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

class GameScene :public Scene//��Ϸ����������
{
public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter()
	{
		status_bar_1P.set_avatar(img_player_1_avatar);//�������ͷ��
		status_bar_2P.set_avatar(img_player_2_avatar);

		status_bar_1P.set_position(235, 625);//���ý�ɫͷ��λ��
		status_bar_2P.set_position(675, 625);

		player_1->set_position(200, 50);//��ʼ����ɫλ��
		player_2->set_position(975, 50);

		pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;//������Ⱦ��ձ���ͼ

		pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;//������Ⱦɽ������ͼ

		platform_list.resize(4);//��������Ϊ4��ƽ̨�б�


		Platform& large_platform = platform_list[0];//����ƽ̨����
		large_platform.img = &img_platform_large;//��ʼ��ͼ��ָ��
		large_platform.render_positon.x = 122;//��ʼ��ƽ̨��Ⱦλ��
		large_platform.render_positon.y = 455;//��ʼ��ƽ̨��Ⱦλ��
		large_platform.shape.left = (float)large_platform.render_positon.x + 30;//��ʼ��ƽ̨����λ��
		large_platform.shape.right = (float)large_platform.render_positon.x + img_platform_large.getwidth() - 30;//��ʼ��ƽ̨����λ��
		large_platform.shape.y = (float)large_platform.render_positon.y + 60;//��ʼ��ƽ̨����߶�ˮƽ�߶�

		Platform& small_platform_1 = platform_list[1];//����ƽ̨����
		small_platform_1.img = &img_platform_small;
		small_platform_1.render_positon.x = 175;
		small_platform_1.render_positon.y = 360;
		small_platform_1.shape.left = (float)small_platform_1.render_positon.x + 40;
		small_platform_1.shape.right = (float)small_platform_1.render_positon.x + img_platform_small.getwidth() - 40;
		small_platform_1.shape.y = (float)small_platform_1.render_positon.y + img_platform_small.getheight() / 2;

		Platform& small_platform_2 = platform_list[2];//����ƽ̨����
		small_platform_2.img = &img_platform_small;
		small_platform_2.render_positon.x = 855;
		small_platform_2.render_positon.y = 360;
		small_platform_2.shape.left = (float)small_platform_2.render_positon.x + 40;
		small_platform_2.shape.right = (float)small_platform_2.render_positon.x + img_platform_small.getwidth() - 40;
		small_platform_2.shape.y = (float)small_platform_2.render_positon.y + img_platform_small.getheight() / 2;

		Platform& small_platform_3 = platform_list[3];//����ƽ̨����
		small_platform_3.img = &img_platform_small;
		small_platform_3.render_positon.x = 515;
		small_platform_3.render_positon.y = 225;
		small_platform_3.shape.left = (float)small_platform_3.render_positon.x + 40;
		small_platform_3.shape.right = (float)small_platform_3.render_positon.x + img_platform_small.getwidth() - 40;
		small_platform_3.shape.y = (float)small_platform_3.render_positon.y + img_platform_small.getheight() / 2;

		//mciSendString(_T("play bgm_game repeat from 0"), NULL, 0, NULL);//����Bgm


	}//��������ʱ��ʼ���߼�

	void on_update(int delta)
	{
		player_1->on_update(delta);
		player_2->on_update(delta);

		main_camera.on_update(delta);//�����������

		bullet_list.erase(std::remove_if(
			bullet_list.begin(), bullet_list.end(),
			[](const Bullet* bullet)
			{
				bool deletable = bullet->check_can_remove();//����Ƿ���Ա�ɾ��
				if (deletable) delete bullet;//ɾ��ָ��Ķ���
				return deletable;//����ָ��//�ø��ϲ�����Ƿ�ɾ��ָ�뱾��
			}),
			bullet_list.end());

		for (Bullet* bullet : bullet_list)//�����ӵ�����
			bullet->on_update(delta);//���ε��ø��·���
	}//��������

	void on_draw(const Camera& camera)
	{
		putimage_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
		putimage_alpha(camera, pos_img_hills.x, pos_img_hills.y, &img_hills);

		for (const Platform& platform : platform_list)//����ÿ��ƽ̨����
			platform.on_draw(camera);//���ε��� on draw ����

		if (is_debug)
		{
			settextcolor(RGB(255, 0, 0));
			outtextxy(15, 15, _T("����ģʽ"));
		}

		player_1->on_draw(camera);//�������ͼ��
		player_2->on_draw(camera);

		for (Bullet* bullet : bullet_list)
			bullet->on_draw(camera);

		status_bar_1P.set_hp(player_1->get_hp());//�������hp mp
		status_bar_1P.set_mp(player_1->get_mp());
		status_bar_2P.set_hp(player_2->get_hp());
		status_bar_2P.set_mp(player_2->get_mp());

		status_bar_1P.on_draw();
		status_bar_2P.on_draw();

	}//��Ⱦ��ͼ

	void on_input(const ExMessage& msg)
	{
		player_1->on_input(msg);//��������Ϣ���ݸ����
		player_2->on_input(msg);

		switch (msg.message)//����Q��������ģʽ
		{
		case WM_KEYUP:
			if (msg.vkcode == 0x51)//Q
				is_debug = !is_debug;
			break;
		default:
			break;
		}
	}//�����������

	void on_exit()
	{
	}//�����˳�ʱ��ж���߼�

private:
	POINT pos_img_sky = { 0 };//�������λ��
	POINT pos_img_hills = { 0 };//ɽ������λ��

	StatusBar status_bar_1P;//���1״̬��
	StatusBar status_bar_2P;//���2״̬��
};