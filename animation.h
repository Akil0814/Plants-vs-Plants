#pragma once
#include"atlas.h"
#include"util.h"
#include"camera.h"

#include<functional>
#include<graphics.h>

class Animation
{
public:
	Animation() = default;
	~Animation() = default;

	void reset()//重置动画状态//重置计时器和动画帧索引
	{
		timer = 0;
		idx_frame = 0;
	}
	void set_atlas(Atlas* new_atlas)//设置动画所使用的图集
	{
		reset();//防止帧索引越界 首先重置动画状态
		atlas = new_atlas;
	}
	void set_loop(bool flag)//设置动画是否循环播放
	{
		is_loop = flag;
	}
	void set_interval(int ms)//设置动画帧间隔
	{
		interval = ms;
	}
	int get_idx_frame()//获取当前帧索引
	{
		return idx_frame;
	}
	IMAGE* get_frame()//获取当前帧图片
	{
		return atlas->get_image(idx_frame);
	}
	bool check_finishend()//检测动画是否结束
	{
		if (is_loop)//若为循环动画 永远不会结束
			return false;

		return (idx_frame == atlas->get_size() - 1);//检测帧索引 是否到达了图集尾部
	}

	void on_update(int delta)//更新动画
	{
		timer += delta;//
		if (timer >= interval)//当计时器到达帧间隔时
		{
			timer = 0;//重置计时器
			idx_frame++;//切换动画帧
			if (idx_frame >= atlas->get_size())//动画帧到达动画帧总数时
			{
				idx_frame = is_loop ? 0 : atlas->get_size() - 1;
				//检测是否为循环//决定是否从头开始//保持在最后一张图片上

				//前置条件：如果帧索引到达图集尾部
				if (!is_loop && callback)//没有循环播放且回调函数存在
					callback();//执行回调函数
			}
		}
	}

	void on_draw(const Camera& camera,int x,int y) const//绘图成员方法//使用const修饰防止在函数内部修改坐标值
	{
		putimage_alpha(camera,x, y, atlas->get_image(idx_frame));
	}

	void set_callback(std::function<void()>callback)
	{
		this->callback = callback;//将传入的函数对象保存到对象本身的callback中
	}

private:
	int timer = 0;//计时器
	int interval = 0;//帧间隔
	int idx_frame = 0;//帧索引
	bool is_loop = true;//是否循环
	Atlas* atlas = nullptr;//
	std::function<void()>callback;//function类型的函数对象
	//function是一个模板类//<>内的内容为储存函数返回值为void

};
