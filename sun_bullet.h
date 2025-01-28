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
		size.x = 96, size.y = 96;//设置尺寸

		damage = 20;//设置伤害

		animation_idle.set_atlas(&atlas_sun);//设置图集
		animation_idle.set_interval(50);//设置帧间隔

		animation_explode.set_atlas(&atlas_sun_explode); // 设置图集
		animation_explode.set_interval(50);//设置帧间隔
		animation_explode.set_loop(false);//设置不可循环
		animation_explode.set_callback([&]() { can_remove = true; });//调用回调函数

		IMAGE* frame_idle = animation_idle.get_frame();//设置动画位置偏移
		IMAGE* frame_explode = animation_explode.get_frame();
		explode_render_offset.x = (frame_idle->getwidth() - frame_explode->getwidth()) / 2.0f;
		explode_render_offset.y = (frame_idle->getheight() - frame_explode->getheight()) / 2.0f;
	}

	~SunBullet() = default;

	void on_collide()
	{
		Bullet::on_collide();

		main_camera.shake(5, 250);//摄像机抖动

		mciSendString(_T("play sun_explode from 0"), NULL, 0, NULL);//播放爆炸音效
	}


	void on_update(int delta)
	{
		if (valid)//子弹有效时
		{
			velocity.y += gravity * delta;//更新子弹速度
			position += velocity * (float)delta;//根据速度修改位置坐标
		}

		if (!valid)
			animation_explode.on_update(delta);//爆炸动画
		else
			animation_idle.on_update(delta);//默认动画

		if (check_if_exceeds_screen())//检测是否可以移除动画
			can_remove = true;
	}

	void on_draw(const Camera& camera) const
	{
		if (valid)//检测子弹是否有效
			animation_idle.on_draw(camera, (int)position.x, (int)position.y);//播放默认动画
		else
		{
			animation_explode.on_draw(camera, //播放爆炸动画
				(int)(position.x + explode_render_offset.x),
				(int)(position.y + explode_render_offset.y));//进行位置偏移
		}

		Bullet::on_draw(camera);
	}
private:
	const float gravity = 1e-3f;//定义日光炸弹重力

private:
	Animation animation_idle;//日光炸弹默认动画
	Animation animation_explode;//日光炸弹爆炸动画
	Vector2 explode_render_offset;//爆炸动画渲染偏移//爆炸动画素材大于默认动画//为确保爆炸动画播放在默认动画中心
};