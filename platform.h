#pragma once
#include"util.h"
#include"camera.h"
#include<graphics.h>

extern bool is_debug;

class Platform
{
public:
	struct CollisionShape//碰撞平台外形
	{
		float y;//竖直坐标位置
		float left, right;//水平方向上左右两点坐标
	};
public:
	CollisionShape shape;//定义变量存储平台外形
	IMAGE* img = nullptr;
	POINT render_positon = { 0 };//图片渲染位置

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