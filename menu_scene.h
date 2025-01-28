#pragma once
#include "scene.h"
#include "scene_manager.h"

extern IMAGE img_menu_background;
extern SceneManager scene_manager;

class MenuScene:public Scene//菜单场景派生类
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter() //场景进入时初始化逻辑
	{
		mciSendString(_T("play bgm_menu repeat from 0"), NULL, 0, NULL);//播放背景音乐
	}
	void on_update(int delta) // 处理数据
	{

	}
	void on_draw(const Camera& camera) //渲染绘图
	{
		putimage(0, 0, &img_menu_background);//渲染背景
	}
	void on_input(const ExMessage& msg) //处理玩家输入
	{
		if (msg.message == WM_KEYUP)
		{
			mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);//播放背景音乐
			scene_manager.switch_to(SceneManager::SceneType::Selector);
		}
	}
private:

};