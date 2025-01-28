#pragma once
#include"util.h"
#include"camera.h"
#include<graphics.h>

extern bool is_debug;

class Platform
{
public:
	struct CollisionShape//��ײƽ̨����
	{
		float y;//��ֱ����λ��
		float left, right;//ˮƽ������������������
	};
public:
	CollisionShape shape;//��������洢ƽ̨����
	IMAGE* img = nullptr;
	POINT render_positon = { 0 };//ͼƬ��Ⱦλ��

public:
	Platform() = default;
	~Platform() = default;

	void on_draw(const Camera& camera) const
	{
		putimage_alpha(camera, render_positon.x, render_positon.y, img);

		if (is_debug)
		{
			setlinecolor(RGB(255, 0, 0));
			line(camera, (int)shape.left, (int)shape.y, (int)shape.right, (int)shape.y);
		}
	}
};