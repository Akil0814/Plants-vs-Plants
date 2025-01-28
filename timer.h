#pragma once
#include<functional>

class Timer
{
public:
	Timer()=default;
	~Timer()=default;

	void on_update(int delta)
	{
		if (paused)
			return;

		pass_time += delta;
		if (pass_time >= wait_time)
		{
			if ((!one_shot || (one_shot && !shotted)) && callback)//检测定时器是否能被触发
			{//如果定时器不是单次触发//或者定时为单次触发并且没有被触发过//同时检测callback是否有效
				callback();//调用回调函数
			}
			shotted = true;//设置为已触发
			pass_time = 0;//重置度过的时间
		}
	}

	void restart()//重置定时器状态
	{
		pass_time = 0;//重新开始计时
		shotted = false;//停止事件的触发
	}

	void set_wait_time(int val)//设置定时器等待时间
	{
		wait_time = val;
	}

	void set_one_shot(bool flag)//设置是否单次触发
	{
		one_shot = flag;
	}

	void set_callback(std::function<void()>callback)//设置回调函数
	{
		this->callback = callback;
	}
	void pause()//定时器的暂停
	{
		paused = true;
	}
	void resume()//定时器的继续
	{
		paused = false;
	}


private:
	int pass_time = 0;                //已过时间
	int wait_time = 0;                //等待时间
	bool paused = false;              //是否暂停
	bool shotted = false;             //是否触发
	bool one_shot = false;            //是否单次触发
	std::function<void()>callback;    //触发回调
};
