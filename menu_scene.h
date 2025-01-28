#pragma once
#include "scene.h"
#include "scene_manager.h"

extern IMAGE img_menu_background;
extern SceneManager scene_manager;

class MenuScene:public Scene//�˵�����������
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter() //��������ʱ��ʼ���߼�
	{
		mciSendString(_T("play bgm_menu repeat from 0"), NULL, 0, NULL);//���ű�������
	}
	void on_update(int delta) // ��������
	{

	}
	void on_draw(const Camera& camera) //��Ⱦ��ͼ
	{
		putimage(0, 0, &img_menu_background);//��Ⱦ����
	}
	void on_input(const ExMessage& msg) //�����������
	{
		if (msg.message == WM_KEYUP)
		{
			mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);//���ű�������
			scene_manager.switch_to(SceneManager::SceneType::Selector);
		}
	}
private:

};