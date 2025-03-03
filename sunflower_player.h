#pragma once
#include"player.h"
#include "sun_bullet.h"
#include "sun_bullet_ex.h"

extern Atlas atlas_sunflower_idle_left;
extern Atlas atlas_sunflower_idle_right;
extern Atlas atlas_sunflower_run_left;
extern Atlas atlas_sunflower_run_right;
extern Atlas atlas_sunflower_attack_ex_left;
extern Atlas atlas_sunflower_attack_ex_right;
extern Atlas atlas_sun_text;
extern Atlas atlas_sunflower_die_left;
extern Atlas atlas_sunflower_die_right;

extern Player* player_1;
extern Player* player_2;

class SunflowerPlayer :public Player
{
public:
	SunflowerPlayer(bool facing_right = true) :Player(facing_right)
	{
		animation_idle_left.set_atlas(&atlas_sunflower_idle_left);//设置对应角色图集
		animation_idle_right.set_atlas(&atlas_sunflower_idle_right);
		animation_run_left.set_atlas(&atlas_sunflower_run_left);
		animation_run_right.set_atlas(&atlas_sunflower_run_right);
		animation_attack_ex_left.set_atlas(&atlas_sunflower_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_sunflower_attack_ex_right);
		animation_sun_text.set_atlas(&atlas_sun_text);
		animation_die_left.set_atlas(&atlas_sunflower_die_left);
		animation_die_right.set_atlas(&atlas_sunflower_die_right);

		animation_idle_left.set_interval(75);//设置动画对象帧间隔
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);
		animation_attack_ex_left.set_interval(100);
		animation_attack_ex_right.set_interval(100);
		animation_sun_text.set_interval(100);
		animation_die_left.set_interval(150);
		animation_die_right.set_interval(150);

		animation_die_left.set_loop(false);
		animation_die_right.set_loop(false);
		animation_attack_ex_left.set_loop(false);
		animation_attack_ex_right.set_loop(false);
		animation_sun_text.set_loop(false);

		animation_attack_ex_left.set_callback([&]()
			{
				is_attacking_ex = false;
				is_sun_text_visible = false;
			});
		animation_attack_ex_right.set_callback([&]()
			{
				is_attacking_ex = false;
				is_sun_text_visible = false;
			});

		size.x = 96, size.y = 96;

		attack_cd = 250;
	}
	~SunflowerPlayer() = default;

	void on_update(int delta)
	{
		Player::on_update(delta);

		if (is_sun_text_visible)//当文本可见时更新文本动画
			animation_sun_text.on_update(delta);
	}

	void on_draw(const Camera& camera)//重写绘图方法
	{
		Player::on_draw(camera);

		if (is_sun_text_visible)
		{
			Vector2 text_position;//存储文本动画渲染位置
			IMAGE* frame = animation_sun_text.get_frame();
			text_position.x = position.x - (size.x - frame->getwidth()) / 2;
			text_position.y = position.y - frame->getheight();
			animation_sun_text.on_draw(camera, (int)text_position.x, (int)text_position.y);//绘制在角色头部居中位置
		}
	}

	void on_attack()
	{
		Bullet* bullet = new SunBullet();//创建小型日光炸弹对象

		Vector2 bullet_position;
		const Vector2& bullet_size = bullet->get_size();
		bullet_position.x = position.x + (size.x - bullet_size.x) / 2;
		bullet_position.y = position.y;//计算子弹位置将生成位置设定为头部

		bullet->set_position(bullet_position.x, bullet_position.y);//设置初始位置
		bullet->set_velocity(is_facing_right ? velocity_sun.x : -velocity_sun.x, velocity_sun.y);//设置初始速度

		bullet->set_collide_target(id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);//设置碰撞目标

		bullet->set_callback([&]() { mp += 35; });

		bullet_list.push_back(bullet);
	}

	void on_attack_ex()
	{
		is_attacking_ex = true;
		is_sun_text_visible = true;

		animation_sun_text.reset();//重置文本动画
		is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();//根据当前朝向重置角色本体动画

		Bullet* bullet = new SunBulletEx();//创建大型日光炸弹对象
		Player* target_player = (id == PlayerID::P1 ? player_2 : player_1);//根据ID判断目标玩家对象

		Vector2 bullet_position, bullet_velocity;//定义速度与位置

		const Vector2& bullet_size = bullet->get_size();
		const Vector2& target_size = target_player->get_size();
		const Vector2& target_position = target_player->get_postion();
		bullet_position.x = target_position.x + (target_size.x - bullet_size.x) / 2;
		bullet_position.y = -size.y;
		bullet_velocity.x = 0;
		bullet_velocity.y = speed_sun_ex;

		bullet->set_position(bullet_position.x, bullet_position.y);
		bullet->set_velocity(bullet_velocity.x, bullet_velocity.y);

		bullet->set_collide_target(id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);//设置碰撞目标

		bullet->set_callback([&]() { mp += 50; });

		bullet_list.push_back(bullet);

		mciSendString(_T("play sun_text from 0"), NULL, 0, NULL);
	}

private:
	const float speed_sun_ex = 0.15f;//大型日光炸弹下落速度
	const Vector2 velocity_sun = { 0.25f, -0.5f };//小型日光炸弹抛射速度

private:
	Animation animation_sun_text;//头顶文字文本动画
	bool is_sun_text_visible = false;//是否显示头顶文本
};