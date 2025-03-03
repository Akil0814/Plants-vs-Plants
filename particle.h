#pragma once

#include "util.h"
#include "atlas.h"
#include "vector2.h"
#include<iostream>


class Particle
{
public:
	Particle() = default;
	Particle(const Vector2& position, Atlas* atlas, int lifespan)
		: position(position), atlas(atlas), lifespan(lifespan)//带参数的重载构造函数//便于初始化
	{ }

	~Particle() = default;

	void set_atlas(Atlas* new_atlas)//设置图集
	{
		atlas = new_atlas;
	}

	void set_position(const Vector2& new_position)//设置坐标
	{
		position = new_position;
	}

	void set_lifespan(int ms)//设置播放时常
	{
		lifespan = ms;
	}

	bool check_valid() const//检测是否有效
	{
		return valid;
	}

	void on_update(int delta)
	{
		timer += delta;
		if (timer >= lifespan)//动画更新逻辑
		{
			timer = 0;
			idx_frame++;
			if (idx_frame >= atlas->get_size())
			{
				idx_frame = atlas->get_size() - 1;//检测到动画播完后
				valid = false;
				std::cout << "particle is invalid" <<std::endl;
			}
		}
	}

	void on_draw(const Camera& camera) const//绘制粒子对象
	{
		putimage_alpha(camera, (int)position.x, (int)position.y, atlas->get_image(idx_frame));
	}

private:
	int timer = 0;               //粒子动画播放定时器
	int lifespan = 0;            //单帧粒子动画持续播放时间
	int idx_frame = 0;           //当前正在播放的动画帧
	Vector2 position;            //粒子的世界坐标位置
	bool valid = true;           //粒子对象是否有效
	Atlas* atlas = nullptr;      //粒子动画所使用的图集

};