#pragma once


#include "scene.h"
#include "player_id.h"
#include "peashooter_player.h"
#include "sunflower_player.h"

extern Player* player_1;//����ָ��
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

class SelectorScene :public Scene//��ҽ�ɫѡ�����
{
public:
	SelectorScene() = default;
	~SelectorScene() = default;

	void on_enter()
	{
		static const int OFFSET_X = 50;

		animation_peashooter.set_atlas(&atlas_peashooter_idle_right);//���ý�ɫͼ��
		animation_sunflower.set_atlas(&atlas_sunflower_idle_right);//���ý�ɫͼ��
		animation_peashooter.set_interval(100);//����֡���
		animation_sunflower.set_interval(100);//����֡���

		pos_img_VS.x = (getwidth() - img_VS.getwidth()) / 2;//������Ϸ�����ز�λ��
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
		animation_peashooter.on_update(delta);//������Ⱦ�Ķ�̬ͼƬ
		animation_sunflower.on_update(delta);//���������ͣ�ڵ�һ֡����֡
		selector_background_scroll_offset_x += 5;//ÿ����Ⱦ����ʱʹ���ƶ�5������
		if (selector_background_scroll_offset_x >= img_peashooter_selector_background_left.getwidth())//���������뵽���ӰͼƬ���ʱ
			selector_background_scroll_offset_x = 0;//����������ֵΪ0
	}

	void on_draw(const Camera& camera)
	{
		IMAGE* img_p1_selector_background = nullptr;//����ָ��ָ����ұ��������ͼƬ����
		IMAGE* img_p2_selector_background = nullptr;

		switch (player_type_2)//ͨ��switch���ƽ�ɫѡ��Ķ�Ӧ�Ľ�ɫ�ļ�Ӱ
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

		switch (player_type_1)//ͨ��switch���ƽ�ɫѡ��Ķ�Ӧ�Ľ�ɫ�ļ�Ӱ
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
		putimage_alpha(getwidth() - selector_background_scroll_offset_x, 0, img_p2_selector_background);//���ƶ�̬����

		putimage_alpha(pos_img_VS.x, pos_img_VS.y, &img_VS);

		putimage_alpha(pos_img_1P.x, pos_img_1P.y, &img_1P);
		putimage_alpha(pos_img_2P.x, pos_img_2P.y, &img_2P);
		putimage_alpha(pos_img_1P_gravestone.x, pos_img_1P_gravestone.y, &img_gravestone_right);//Ĺ��ͼƬ����
		putimage_alpha(pos_img_2P_gravestone.x, pos_img_2P_gravestone.y, &img_gravestone_left);


		switch (player_type_1)//���ݽ�ɫѡ���ڶ�Ӧλ����Ⱦ��ӦͼƬ
		{
		case PlayerType::Peashooter:
			animation_peashooter.on_draw(camera, pos_animation_1P.x, pos_animation_1P.y);
			pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_peashooter_name)) / 2;//���û�ͼλ��
			outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_peashooter_name);//����ѡ��Ľ�ɫ�����ı�����
			break;
		case PlayerType::Sunflower:
			animation_sunflower.on_draw(camera, pos_animation_1P.x, pos_animation_1P.y);
			pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_sunflower_name)) / 2;//���û�ͼλ��
			outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_sunflower_name);//����ѡ��Ľ�ɫ�����ı�����
			break;
		}

		switch (player_type_2)//���ݽ�ɫѡ���ڶ�Ӧλ����Ⱦ��ӦͼƬ
		{
		case PlayerType::Peashooter:
			animation_peashooter.on_draw(camera, pos_animation_2P.x, pos_animation_2P.y);
			pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_left.getwidth() - textwidth(str_peashooter_name)) / 2;//���û�ͼλ��
			outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_peashooter_name);//����ѡ��Ľ�ɫ�����ı�����
			break;
		case PlayerType::Sunflower:
			animation_sunflower.on_draw(camera, pos_animation_2P.x, pos_animation_2P.y);
			pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_left.getwidth() - textwidth(str_sunflower_name)) / 2;//���û�ͼλ��
			outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_sunflower_name);//����ѡ��Ľ�ɫ�����ı�����
			break;
		}

		putimage_alpha(pos_img_1P_desc.x, pos_img_1P_desc.y, &img_1P_desc);
		putimage_alpha(pos_img_2P_desc.x, pos_img_2P_desc.y, &img_2P_desc);

		putimage_alpha(pos_1P_selector_btn_left.x, pos_1P_selector_btn_left.y, is_btn_1P_left_down ? &img_1P_selector_btn_down_left : &img_1P_selector_btn_idle_left);//��ɫѡ�����
		putimage_alpha(pos_1P_selector_btn_right.x, pos_1P_selector_btn_right.y, is_btn_1P_right_down ? &img_1P_selector_btn_down_right : &img_1P_selector_btn_idle_right);
		putimage_alpha(pos_2P_selector_btn_left.x, pos_2P_selector_btn_left.y, is_btn_2P_left_down ? &img_2P_selector_btn_down_left : &img_2P_selector_btn_idle_left);
		putimage_alpha(pos_2P_selector_btn_right.x, pos_2P_selector_btn_right.y, is_btn_2P_right_down ? &img_2P_selector_btn_down_right : &img_2P_selector_btn_idle_right);

		putimage_alpha(pos_img_tip.x, pos_img_tip.y, &img_selector_tip);//��������
	}

	void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN://�������µ���Ϣ����
			switch (msg.vkcode)//���ݰ������ͽ���case
			{
			case 0x41:// 'A'
				is_btn_1P_left_down = true;//���ð���Ϊtrue
				break;
			case 0x44:// 'D'
				is_btn_1P_right_down = true;
				break;
			case VK_LEFT:// '��'
				is_btn_2P_left_down = true;
				break;
			case VK_RIGHT:// '��'
				is_btn_2P_right_down = true;
				break;
			}
			break;
		case WM_KEYUP://����̧�����Ϣ����
			switch (msg.vkcode)//���ݰ������ͽ��� case
			{
				
			case 0x41:// 'A'
				is_btn_1P_left_down = false;//���ð���Ϊ false
				player_type_1 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_1 - 1) % (int)PlayerType::Invalid);
				//��InvalidҲת��Ϊ intֵ������ ����ҽ�ɫö�ٱ���ת��Ϊ intֵ��-1//ȷ����������ֵ��Ӵ���0��
				//���õ�ֵ ��Invalid��Ӧ��intֵ ����ȡģ����ȷ�����յ�ֵ������ڻ����Invalid//���õ���ö�����͵�ֵ��ֵ���������
				//ȷ���õ�ֵ��ö�����һ�������һ��ֵ֮�� �� [peashooter,Invalid) ��������
				mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				break;
				
			case 0x44:// 'D'
				is_btn_1P_right_down = false;
				player_type_1 = (PlayerType)(((int)player_type_1 + 1) % (int)PlayerType::Invalid);
				mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				break;
				
			case VK_LEFT:// '��'
				is_btn_2P_left_down = false;
				player_type_2 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_2 - 1) % (int)PlayerType::Invalid);
				mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				break;
				
			case VK_RIGHT:// '��'
				is_btn_2P_right_down = false;
				player_type_2 = (PlayerType)(((int)player_type_2 + 1) % (int)PlayerType::Invalid);
				mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				break;
			case VK_RETURN://�س���
				scene_manager.switch_to(SceneManager::SceneType::Game);//�л���Ϸ����
				mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
				break;
			}
			break;
		}
	}

	void on_exit()
	{
		switch (player_type_1)//�������ѡ��ͬ�Ľ�ɫ����
		{
		case PlayerType::Peashooter:
			player_1 = new PeashooterPlayer();//ʵ������ͬ�Ľ�ɫ�������
			img_player_1_avatar = &img_avatar_peashooter;//�������ͷ��

			break;
		case PlayerType::Sunflower:
			player_1 = new SunflowerPlayer();
			img_player_1_avatar = &img_avatar_sunflower;//�������ͷ��

			break;
		}
		player_1->set_id(PlayerID::P1);//����Ҷ��󴴽���ΪID��ֵ

		switch (player_type_2)//�������ѡ��ͬ�Ľ�ɫ����
		{
		case PlayerType::Peashooter:
			player_2 = new PeashooterPlayer();
			img_player_2_avatar = &img_avatar_peashooter;//�������ͷ��

			break;
		case PlayerType::Sunflower:
			player_2 = new SunflowerPlayer();
			img_player_2_avatar = &img_avatar_sunflower;//�������ͷ��

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
	bool is_btn_1P_left_down = false;//����Ƿ����˶�Ӧ����İ�ť
	bool is_btn_1P_right_down = false;
	bool is_btn_2P_left_down = false;
	bool is_btn_2P_right_down = false;
	POINT pos_img_VS = { 0 };// VS ������ͼƬλ��
	POINT pos_img_tip = { 0 };// ��ʾ��Ϣ�ı�ͼƬλ��
	POINT pos_img_1P = { 0 };// 1P �ı�ͼƬλ��
	POINT pos_img_2P = { 0 };// 2P �ı�ͼƬλ��
	POINT pos_img_1P_desc = { 0 };// 1P ��λ����ͼƬλ��
	POINT pos_img_2P_desc = { 0 };// 2P ��λ����ͼƬλ��
	POINT pos_img_1P_name = { 0 };// 1P ��ɫ�����ı�λ��
	POINT pos_img_2P_name = { 0 };// 2P ��ɫ�����ı�λ��
	POINT pos_animation_1P = { 0 };// 1P ��ɫ����λ��
	POINT pos_animation_2P = { 0 };// 2P ��ɫ����λ��
	POINT pos_img_1P_gravestone = { 0 };// 1P Ĺ��ͼƬλ��
	POINT pos_img_2P_gravestone = { 0 };// 2P Ĺ��ͼƬλ��
	POINT pos_1P_selector_btn_left = { 0 };// 1P �����л���ťλ��
	POINT pos_1P_selector_btn_right = { 0 };// 1P �����л���ťλ��
	POINT pos_2P_selector_btn_left = { 0 };// 2P �����л���ťλ��
	POINT pos_2P_selector_btn_right = { 0 };// 2P �����л���ťλ��
	Animation animation_peashooter;//�����ֽ�ɫ����
	Animation animation_sunflower;//���տ���ɫ����
	PlayerType player_type_1 = PlayerType::Peashooter;//�����������
	PlayerType player_type_2 = PlayerType::Sunflower;

	LPCTSTR str_peashooter_name = _T("���㶹���֡�");//�����ɫ���ı��ַ���
	LPCTSTR str_sunflower_name = _T("�����տ���");

	int selector_background_scroll_offset_x = 0;//��������ˮƽ�����Ϲ����ľ���

private:
	void outtextxy_shaded(int x, int y, LPCTSTR str)//���ƴ���ӰЧ�����ı�
	{
		settextcolor(RGB(45, 45, 45));
		outtextxy(x + 3, y + 3, str);
		settextcolor(RGB(255, 255, 255));
		outtextxy(x, y, str);
	}
};

