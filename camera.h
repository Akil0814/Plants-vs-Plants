#pragma once
#include"vector2.h"
#include"timer.h"


class Camera
{
public:
	Camera()
	{
		timer_shake.set_one_shot(true);//设置为单次触发
		timer_shake.set_callback([&]()
			{
				is_shaking = false;//取消抖动状态
				reset();//重置摄像机位置
			});
	}
	~Camera()=default;

	const Vector2& get_position() const
	{
		return position;
	}

	void reset()//将摄像机位置归零
	{
		position.x = 0;
		position.y = 0;
	}

	void on_update(int delta)
	{
		timer_shake.on_update(delta);

		if (is_shaking)
		{
			position.x = (-50 + rand() % 100) / 50.0f * shaking_strength;
			position.y = (-50 + rand() % 100) / 50.0f * shaking_strength;

		}

	}

	void shake(float strength, int duration)//设置摄像机抖动的强度和时间
	{
		is_shaking = true;//设置是否抖动为true		
		shaking_strength = strength;//设置抖动强度

		timer_shake.set_wait_time(duration);//持续时间为抖动时间
		timer_shake.restart();//重新开始计时
	}

private:
	Vector2 position;//摄像机位置属性
	Timer timer_shake;//摄像机抖动定时器
	bool is_shaking = false;//摄像机是否在抖动
	float shaking_strength = 0;//摄像机抖动幅度
};
