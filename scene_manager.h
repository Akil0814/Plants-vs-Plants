#pragma once
#include"scene.h"


extern Scene* menu_scene;//extern�����ⲿ����
extern Scene* game_scene;//��ȡȫ��ָ�������
extern Scene* selector_scene;

class SceneManager//����������
{
public:
	enum class SceneType//��ǵ�ǰ����״̬
	{
		Menu,
		Game,
		Selector
	};
public:
	SceneManager() = default;
	~SceneManager() = default;

	void set_current_scene(Scene* scene)//���õ�ǰ����
	{
		current_scene = scene;//ʹ��ָ��ָ��ĳ���״̬
		current_scene->on_enter();//���õ�ǰ������on enter����
	}

	void switch_to(SceneType type)
	{
		current_scene->on_exit();//���õ�ǰ������on exit����
		switch (type)//����ö����Ĳ�ͬ ��ת����ͬ�Ķ�Ӧ�׶�
		{
		case SceneType::Menu:
			current_scene = menu_scene;//���ĵ�ǰ scene����
			break;
		case SceneType::Game:
			current_scene = game_scene;//���ĵ�ǰ scene����

			break;
		case SceneType::Selector:
			current_scene = selector_scene;//���ĵ�ǰ scene����
			break;
		default:
			break;
		}
		current_scene->on_enter();//���ø��ĺ�� scene���͵�on enter����

	}


	void on_update(int delta)
	{
		current_scene->on_update(delta);
	}//��������
	void on_draw(const Camera& camera)
	{
		current_scene->on_draw(camera);
	}//��Ⱦ��ͼ
	void on_input(const ExMessage& msg)
	{
		current_scene->on_input(msg);
	}//�����������


private:
	Scene* current_scene = nullptr;
};