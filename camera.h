#pragma once
#include"vector2.h"
#include"timer.h"


class Camera
{
public:
	Camera()
	{
		timer_shake.set_one_shot(true);//����Ϊ���δ���
		timer_shake.set_callback([&]()
			{
				is_shaking = false;//ȡ������״̬
				reset();//���������λ��
			});
	}
	~Camera()=default;

	const Vector2& get_position() const
	{
		return position;
	}

	void reset()//�������λ�ù���
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

	void shake(float strength, int duration)//���������������ǿ�Ⱥ�ʱ��
	{
		is_shaking = true;//�����Ƿ񶶶�Ϊtrue		
		shaking_strength = strength;//���ö���ǿ��

		timer_shake.set_wait_time(duration);//����ʱ��Ϊ����ʱ��
		timer_shake.restart();//���¿�ʼ��ʱ
	}

private:
	Vector2 position;//�����λ������
	Timer timer_shake;//�����������ʱ��
	bool is_shaking = false;//������Ƿ��ڶ���
	float shaking_strength = 0;//�������������
};
