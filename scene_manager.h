#pragma once
#include"scene.h"


extern Scene* menu_scene;//extern声明外部变量
extern Scene* game_scene;//获取全局指针的引用
extern Scene* selector_scene;

class SceneManager//场景管理器
{
public:
	enum class SceneType//标记当前场景状态
	{
		Menu,
		Game,
		Selector
	};
public:
	SceneManager() = default;
	~SceneManager() = default;

	void set_current_scene(Scene* scene)//设置当前场景
	{
		current_scene = scene;//使用指针指向的场景状态
		current_scene->on_enter();//调用当前场景的on enter方法
	}

	void switch_to(SceneType type)
	{
		current_scene->on_exit();//调用当前场景的on exit方法
		switch (type)//根据枚举类的不同 跳转到不同的对应阶段
		{
		case SceneType::Menu:
			current_scene = menu_scene;//更改当前 scene类型
			break;
		case SceneType::Game:
			current_scene = game_scene;//更改当前 scene类型

			break;
		case SceneType::Selector:
			current_scene = selector_scene;//更改当前 scene类型
			break;
		default:
			break;
		}
		current_scene->on_enter();//调用更改后的 scene类型的on enter方法

	}


	void on_update(int delta)
	{
		current_scene->on_update(delta);
	}//处理数据
	void on_draw(const Camera& camera)
	{
		current_scene->on_draw(camera);
	}//渲染绘图
	void on_input(const ExMessage& msg)
	{
		current_scene->on_input(msg);
	}//处理玩家输入


private:
	Scene* current_scene = nullptr;
};