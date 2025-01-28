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
		size.x = 64, size.y = 64;//设置子弹大小

		damage = 10;//设置子弹伤害

		animation_break.set_atlas(&atlas_pea_break);//设置破碎动画使用的图集
		animation_break.set_interval(100);//设置帧间隔
		animation_break.set_loop(false);//动画是否循环
		animation_break.set_callback([&]() { can_remove = true; });//破碎动画播放结束后设置子弹为可被移除
	}
	~PeaBullet()=default;

	void on_collide()
	{
		Bullet::on_collide();//重新父类方法 执行父类逻辑->调用父类方法

		switch (rand() % 3)//随机播放三种音效
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
		position += velocity * (float)delta;//更新子弹位置

		if (!valid)//检测子弹是否有效
			animation_break.on_update(delta);//若已经失效则播放破碎动画

		if (check_if_exceeds_screen())//检测子弹是否已经飞出屏幕
			can_remove = true;
	}


	void on_draw(const Camera& camera) const
	{
		if (valid)
			putimage_alpha(camera, (int)position.x, (int)position.y, &img_pea);//绘制子弹图片
		else
			animation_break.on_draw(camera, (int)position.x, (int)position.y);//播放破碎动画

		Bullet::on_draw(camera);
	}

private:
	Animation animation_break;//豌豆子弹破碎动画

};
