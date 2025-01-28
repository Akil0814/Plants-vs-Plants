#pragma once

#include "bullet.h"
#include "animation.h"
#include "camera.h"

extern Atlas atlas_sun;
extern Atlas atlas_sun_explode;

extern Camera main_camera;

class SunBullet : public Bullet
{
public:
	SunBullet()
	{
		size.x = 96, size.y = 96;//���óߴ�

		damage = 20;//�����˺�

		animation_idle.set_atlas(&atlas_sun);//����ͼ��
		animation_idle.set_interval(50);//����֡���

		animation_explode.set_atlas(&atlas_sun_explode); // ����ͼ��
		animation_explode.set_interval(50);//����֡���
		animation_explode.set_loop(false);//���ò���ѭ��
		animation_explode.set_callback([&]() { can_remove = true; });//���ûص�����

		IMAGE* frame_idle = animation_idle.get_frame();//���ö���λ��ƫ��
		IMAGE* frame_explode = animation_explode.get_frame();
		explode_render_offset.x = (frame_idle->getwidth() - frame_explode->getwidth()) / 2.0f;
		explode_render_offset.y = (frame_idle->getheight() - frame_explode->getheight()) / 2.0f;
	}

	~SunBullet() = default;

	void on_collide()
	{
		Bullet::on_collide();

		main_camera.shake(5, 250);//���������

		mciSendString(_T("play sun_explode from 0"), NULL, 0, NULL);//���ű�ը��Ч
	}


	void on_update(int delta)
	{
		if (valid)//�ӵ���Чʱ
		{
			velocity.y += gravity * delta;//�����ӵ��ٶ�
			position += velocity * (float)delta;//�����ٶ��޸�λ������
		}

		if (!valid)
			animation_explode.on_update(delta);//��ը����
		else
			animation_idle.on_update(delta);//Ĭ�϶���

		if (check_if_exceeds_screen())//����Ƿ�����Ƴ�����
			can_remove = true;
	}

	void on_draw(const Camera& camera) const
	{
		if (valid)//����ӵ��Ƿ���Ч
			animation_idle.on_draw(camera, (int)position.x, (int)position.y);//����Ĭ�϶���
		else
		{
			animation_explode.on_draw(camera, //���ű�ը����
				(int)(position.x + explode_render_offset.x),
				(int)(position.y + explode_render_offset.y));//����λ��ƫ��
		}

		Bullet::on_draw(camera);
	}
private:
	const float gravity = 1e-3f;//�����չ�ը������

private:
	Animation animation_idle;//�չ�ը��Ĭ�϶���
	Animation animation_explode;//�չ�ը����ը����
	Vector2 explode_render_offset;//��ը������Ⱦƫ��//��ը�����زĴ���Ĭ�϶���//Ϊȷ����ը����������Ĭ�϶�������
};