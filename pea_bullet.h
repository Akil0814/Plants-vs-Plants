#pragma once
#include "bullet.h"
#include "animation.h"

extern IMAGE img_pea;
extern Atlas atlas_pea_break;

class PeaBullet : public Bullet
{
public:
	PeaBullet()
	{
		size.x = 64, size.y = 64;//�����ӵ���С

		damage = 10;//�����ӵ��˺�

		animation_break.set_atlas(&atlas_pea_break);//�������鶯��ʹ�õ�ͼ��
		animation_break.set_interval(100);//����֡���
		animation_break.set_loop(false);//�����Ƿ�ѭ��
		animation_break.set_callback([&]() { can_remove = true; });//���鶯�����Ž����������ӵ�Ϊ�ɱ��Ƴ�
	}
	~PeaBullet()=default;

	void on_collide()
	{
		Bullet::on_collide();//���¸��෽�� ִ�и����߼�->���ø��෽��

		switch (rand() % 3)//�������������Ч
		{
		case 0:
			mciSendString(_T("play pea_break_1 from 0"), NULL, 0, NULL);
			break;
		case 1:
			mciSendString(_T("play pea_break_2 from 0"), NULL, 0, NULL);
			break;
		case 2:
			mciSendString(_T("play pea_break_3 from 0"), NULL, 0, NULL);
			break;
		}
	}

	void on_update(int delta)
	{
		position += velocity * (float)delta;//�����ӵ�λ��

		if (!valid)//����ӵ��Ƿ���Ч
			animation_break.on_update(delta);//���Ѿ�ʧЧ�򲥷����鶯��

		if (check_if_exceeds_screen())//����ӵ��Ƿ��Ѿ��ɳ���Ļ
			can_remove = true;
	}


	void on_draw(const Camera& camera) const
	{
		if (valid)
			putimage_alpha(camera, (int)position.x, (int)position.y, &img_pea);//�����ӵ�ͼƬ
		else
			animation_break.on_draw(camera, (int)position.x, (int)position.y);//�������鶯��

		Bullet::on_draw(camera);
	}

private:
	Animation animation_break;//�㶹�ӵ����鶯��

};
