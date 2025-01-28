#pragma once
#include"player.h"
#include "pea_bullet.h"

extern Atlas atlas_peashooter_idle_left;
extern Atlas atlas_peashooter_idle_right;
extern Atlas atlas_peashooter_run_left;
extern Atlas atlas_peashooter_run_right;
extern Atlas atlas_peashooter_attack_ex_left;
extern Atlas atlas_peashooter_attack_ex_right;



class PeashooterPlayer:public Player
{
public:
	PeashooterPlayer()
	{
		animation_idle_left.set_atlas(&atlas_peashooter_idle_left);//设置对应角色图集
		animation_idle_right.set_atlas(&atlas_peashooter_idle_right);
		animation_run_left.set_atlas(&atlas_peashooter_run_left);
		animation_run_right.set_atlas(&atlas_peashooter_run_right);
		animation_attack_ex_left.set_atlas(&atlas_peashooter_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_peashooter_attack_ex_right);

		animation_idle_left.set_interval(75);//设置动画对象帧间隔
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);
		animation_attack_ex_left.set_interval(75);
		animation_attack_ex_right.set_interval(75);

		size.x = 96, size.y = 96;

		timer_attack_ex.set_wait_time(attack_ex_duration);//设置特殊攻击定时器
		timer_attack_ex.set_one_shot(true);
		timer_attack_ex.set_callback([&]()
			{
				is_attacking_ex = false;
			});

		timer_spwan_pea_ex.set_wait_time(100);
		timer_spwan_pea_ex.set_callback([&]()
			{
				spawn_pea_bullet(speed_pea_ex);
			});

		attack_cd = 100;//普攻cd

	}
	~PeashooterPlayer() = default;

	void on_update(int delta)
	{
		Player::on_update(delta);

		if (is_attacking_ex)//根据是否在特殊攻击抖动摄像机
		{
			main_camera.shake(5, 100);
			timer_attack_ex.on_update(delta);
			timer_spwan_pea_ex.on_update(delta);
		}
	}

	void on_attack()
	{
		spawn_pea_bullet(speed_pea);

		switch (rand() % 2)//随机播放两种音效
		{
		case 0:
			mciSendString(_T("play pea_shoot_1 from 0"), NULL, 0, NULL);
			break;
		case 1:
			mciSendString(_T("play pea_shoot_2 from 0"), NULL, 0, NULL);
			break;
		}
	}

	void on_attack_ex()
	{
		is_attacking_ex = true;
		timer_attack_ex.restart();

		is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();//根据角色朝向播放对应动画

		mciSendString(_T("play pea_shoot_ex from 0"), NULL, 0, NULL);
	}

private:
	Timer timer_attack_ex;//特殊攻击状态定时器
	Timer timer_spwan_pea_ex;//子弹发射定时器

private:
	const float speed_pea = 0.75f;//设置子弹速度
	const float speed_pea_ex = 1.5f;
	const int attack_ex_duration = 2500;//特殊攻击持续时长

private:

	void spawn_pea_bullet(float speed)//生成子弹逻辑
	{
		Bullet* bullet = new PeaBullet();//创建豌豆子弹对象

		Vector2 bullet_position, bullet_velocity;//定义子弹位置
		const Vector2& bullet_size = bullet->get_size();//获取子弹尺寸
		bullet_position.x = 
			is_facing_right ? position.x + size.x - bullet_size.x / 2: position.x - bullet_size.x / 2;//根据角色位置判断子弹朝向
		bullet_position.y = position.y;
		bullet_velocity.x = is_facing_right ? speed : -speed;//根据角色朝向设置水平速度
		bullet_velocity.y = 0;//设置子弹竖直速度为0

		bullet->set_position(bullet_position.x, bullet_position.y);//将计算得到的值设置到子弹对象中
		bullet->set_velocity(bullet_velocity.x, bullet_velocity.y);

		bullet->set_collide_target(id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);//设置子弹碰撞目标为对方角色

		bullet->set_callback([&]() { mp += 25; });//子弹在碰撞后的回调函数

		bullet_list.push_back(bullet);//将子弹对象添加到全局bullet对象中
	}

};