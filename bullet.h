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

	void set_damage(int val)//�����ӵ�������ɵ��˺�
	{
		damage = val;
	}

	int get_damage()//��ȡ�ӵ�������ɵ��˺�
	{
		return damage;
	}

	void set_position(float x, float y)//�����ӵ�λ��
	{
		position.x = x, position.y = y;
	}

	const Vector2& get_position() const//��ȡ�ӵ�λ��
	{
		return position;
	}

	const Vector2& get_size() const//��ȡ�ӵ���С
	{
		return size;
	}

	void set_velocity(float x,float y)//�����ӵ��ٶ�
	{
		velocity.x = x, velocity.y = y;
	}

	void set_collide_target(PlayerID target)//������ײĿ��
	{
		target_id = target;
	}

	PlayerID get_collide_target() const//��ȡ��ײĿ��
	{
		return target_id;
	}

	void set_callback(std::function<void()>callback)//��ײ��Ļص�
	{
		this->callback = callback;
	}

	void set_valid(bool flag)//�����ӵ�
	{
		valid = flag;
	}

	bool get_valid()const//����ӵ��Ƿ���Լ�����ײ
	{
		return valid;
	}

	bool check_can_remove() const//����Ƿ�����Ƴ��ӵ�����
	{
		return can_remove;
	}

	virtual void on_collide()//�ӵ�������ײʱ����
	{
		if (callback)
			callback();
	}

	virtual bool check_collision(const Vector2& position, const Vector2& size)//����ӵ��Ƿ���ָ��λ����ߴ��Ŀ�귢����ײ
	{
		return this->position.x + this->size.x / 2 >= position.x
			&& this->position.x + this->size.x / 2 <= position.x + size.x
			&& this->position.y + this->size.y / 2 >= position.y
			&& this->position.y + this->size.y / 2 <= position.y + size.y;
	}

	virtual void on_update(int delta) {}//�ӵ���ͼ�߼�

	virtual void on_draw(const Camera& camera) const//����ģʽ
	{
		if (is_debug)//�����ӵ���ײģ��
		{
			setfillcolor(RGB(255, 255, 255));
			setlinecolor(RGB(255, 255, 255));
			rectangle((int)position.x, (int)position.y,
				(int)(position.x + size.x), (int)(position.y + size.y));
			solidcircle((int)(position.x + size.x / 2), (int)(position.y + size.y / 2), 5);
		}
	}
protected:
	Vector2 size;                        //�ӵ��ߴ�
	Vector2 position;                    //�ӵ�λ��
	Vector2 velocity;                    //�ӵ��ٶ�
	int damage = 10;                     //�ӵ��˺�
	bool valid = true;                   //�ӵ��Ƿ���Ч
	bool can_remove = false;             //�ӵ��Ƿ����ɾ��
	std::function<void()>callback;       //�ӵ���ײ�ص�����
	PlayerID target_id = PlayerID::P1;   //�ӵ���ײĿ�����ID

protected:
	bool check_if_exceeds_screen()//����ӵ��Ƿ�λ����Ļ��
	{
		return (position.x + size.x <= 0 || position.x >= getwidth()
			|| position.y + size.y <= 0 || position.y >= getheight());
	}
};