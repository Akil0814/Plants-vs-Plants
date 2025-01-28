#pragma once
#include"vector2.h"
#include"camera.h"
#include"player_id.h"

#include<functional>
#include<graphics.h>

extern bool is_debug;

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;

	void set_damage(int val)//设置子弹可以造成的伤害
	{
		damage = val;
	}

	int get_damage()//获取子弹可以造成的伤害
	{
		return damage;
	}

	void set_position(float x, float y)//设置子弹位置
	{
		position.x = x, position.y = y;
	}

	const Vector2& get_position() const//获取子弹位置
	{
		return position;
	}

	const Vector2& get_size() const//获取子弹大小
	{
		return size;
	}

	void set_velocity(float x,float y)//设置子弹速度
	{
		velocity.x = x, velocity.y = y;
	}

	void set_collide_target(PlayerID target)//设置碰撞目标
	{
		target_id = target;
	}

	PlayerID get_collide_target() const//获取碰撞目标
	{
		return target_id;
	}

	void set_callback(std::function<void()>callback)//碰撞后的回调
	{
		this->callback = callback;
	}

	void set_valid(bool flag)//设置子弹
	{
		valid = flag;
	}

	bool get_valid()const//检测子弹是否可以继续碰撞
	{
		return valid;
	}

	bool check_can_remove() const//检测是否可以移除子弹对象
	{
		return can_remove;
	}

	virtual void on_collide()//子弹发生碰撞时调用
	{
		if (callback)
			callback();
	}

	virtual bool check_collision(const Vector2& position, const Vector2& size)//检测子弹是否与指定位置与尺寸的目标发生碰撞
	{
		return this->position.x + this->size.x / 2 >= position.x
			&& this->position.x + this->size.x / 2 <= position.x + size.x
			&& this->position.y + this->size.y / 2 >= position.y
			&& this->position.y + this->size.y / 2 <= position.y + size.y;
	}

	virtual void on_update(int delta) {}//子弹绘图逻辑

	virtual void on_draw(const Camera& camera) const//调试模式
	{
		if (is_debug)//绘制子弹碰撞模型
		{
			setfillcolor(RGB(255, 255, 255));
			setlinecolor(RGB(255, 255, 255));
			rectangle((int)position.x, (int)position.y,
				(int)(position.x + size.x), (int)(position.y + size.y));
			solidcircle((int)(position.x + size.x / 2), (int)(position.y + size.y / 2), 5);
		}
	}
protected:
	Vector2 size;                        //子弹尺寸
	Vector2 position;                    //子弹位置
	Vector2 velocity;                    //子弹速度
	int damage = 10;                     //子弹伤害
	bool valid = true;                   //子弹是否有效
	bool can_remove = false;             //子弹是否可以删除
	std::function<void()>callback;       //子弹碰撞回调函数
	PlayerID target_id = PlayerID::P1;   //子弹碰撞目标玩家ID

protected:
	bool check_if_exceeds_screen()//检测子弹是否位于屏幕外
	{
		return (position.x + size.x <= 0 || position.x >= getwidth()
			|| position.y + size.y <= 0 || position.y >= getheight());
	}
};