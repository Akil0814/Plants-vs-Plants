#pragma once

#include"util.h"

class StatusBar
{
public:
	StatusBar() = default;
	~StatusBar() = default;

	void set_avatar(IMAGE* img)//设置显示图像
	{
		img_avatar = img;
	}

	void set_position(int x, int y)//设置显示位置
	{
		position.x = x, position.y = y;
	}

	void set_hp(int val)
	{
		hp = val;
	}

	void set_mp(int val)
	{
		mp = val;
	}

	void on_draw()
	{
		putimage_alpha(position.x, position.y, img_avatar);//绘制角色头像

		setfillcolor(RGB(5, 5, 5));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + width + 3 * 2, position.y + 36, 8, 8);
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + width + 3 * 2, position.y + 71, 8, 8);
		setfillcolor(RGB(67, 47, 47));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + width + 3, position.y + 33, 8, 8);
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + width + 3, position.y + 68, 8, 8);//绘制状态条

		float hp_bar_width = width * max(0, hp) / 100.0f;//设置状态条
		float mp_bar_width = width * min(100, mp) / 100.0f;//设置状态条

		setfillcolor(RGB(197, 61, 67));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + (int)hp_bar_width + 3, position.y + 33, 8, 8);
		setfillcolor(RGB(83, 131, 195));
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + (int)mp_bar_width + 3, position.y + 68, 8, 8);

	}


private:
	const int width = 275;          //状态条宽度

	int hp = 0;						//需要显示的生命值信息
	int mp = 0;						//需要显示的能量值信息
	POINT position = { 0 };			//在窗口中显示的位置
	IMAGE* img_avatar = nullptr;	//角色头像图片
};