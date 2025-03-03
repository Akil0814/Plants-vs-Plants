 #pragma once

#include "camera.h"
#include "player_id.h"
#include "vector2.h"
#include "animation.h"
#include "platform.h"
#include "bullet.h"
#include "particle.h"
#include <iostream>
#include <graphics.h>


extern bool is_debug;

extern std::vector<Platform>platform_list;
extern std::vector<Bullet*> bullet_list;

extern Atlas atlas_run_effect;
extern Atlas atlas_jump_effect;
extern Atlas atlas_land_effect;

extern IMAGE img_1P_cursor;
extern IMAGE img_2P_cursor;

class Player
{
public:
	Player(bool facing_right=true):is_facing_right(facing_right)
	{
		current_animation = &animation_idle_right;//设置默认方向

		timer_attack_cd.set_wait_time(attack_cd);//初始化普攻冷却定时器
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]()
			{
				can_attack = true;
			});

		timer_invulnerable.set_wait_time(750);//设置无敌时间
		timer_invulnerable.set_one_shot(true);
		timer_invulnerable.set_callback([&]()
			{
				is_invulnerable = false;
			});

		timer_invulnerable_blink.set_wait_time(75);//设置无敌帧切换速度
		timer_invulnerable_blink.set_callback([&]()
			{
				is_showing_sketch_frame = !is_showing_sketch_frame;
			});

		timer_run_effect_generation.set_wait_time(75);//跑动粒子特效定时器
		timer_run_effect_generation.set_callback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();//根据玩家位置动态计算粒子位置
				particle_list.emplace_back(particle_position, &atlas_run_effect, 45);
			});

		timer_die_effect_generation.set_wait_time(35);//死亡特效粒子定时器
		timer_die_effect_generation.set_callback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				particle_list.emplace_back(particle_position, &atlas_run_effect, 150);
			});

		animation_jump_effect.set_atlas(&atlas_jump_effect);//跳跃特效动画
		animation_jump_effect.set_interval(25);
		animation_jump_effect.set_loop(false);
		animation_jump_effect.set_callback([&]()
			{
				is_jump_effect_visible = false;
			});

		animation_land_effect.set_atlas(&atlas_land_effect);//落地特效动画
		animation_land_effect.set_interval(50);
		animation_land_effect.set_loop(false);
		animation_land_effect.set_callback([&]()
			{
				is_land_effect_visible = false;
			});

		timer_cursor_visibility.set_wait_time(2500);
		timer_cursor_visibility.set_one_shot(true);
		timer_cursor_visibility.set_callback([&]()
			{
				is_cursor_visible = false;
			});
	}
	~Player() = default;

	virtual void on_update(int delta)
	{
		int direction = is_right_key_down - is_left_key_down;//做差判断角色朝向

		if (direction != 0)//当 direction的值不为0时
		{
			if (!is_attacking_ex)//没有进行特殊攻击时
			   is_facing_right = direction > 0;//修改is_facing_right的值
			current_animation = is_facing_right ? &animation_run_right : &animation_run_left;//根据玩家朝向播放不同动画
			float distance = direction * run_velocity * delta;//方向*跑动速度*时间
			on_run(distance);//将移动距离传给 run方法
		}
		else
		{
			current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;//没有移动时选择播放的默认动画
			timer_run_effect_generation.pause();//当角色不在跑动时停止跑动粒子的生成
		}

		if (is_attacking_ex)//根据角色特殊攻击状态
			current_animation = is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left;//决定播放对应方向的动画

		if (hp <= 0)//生命值归零时播放对应动画
			current_animation = last_hurt_direction.x < 0 ? &animation_die_left : &animation_die_right;

		current_animation->on_update(delta);//对当前播放的动画进行更新
		animation_jump_effect.on_update(delta);//更新跳跃特效动画
		animation_land_effect.on_update(delta);//更新落地特效动画

		timer_attack_cd.on_update(delta);//更新定时器
		timer_invulnerable.on_update(delta);
		timer_invulnerable_blink.on_update(delta);
		timer_run_effect_generation.on_update(delta);
		timer_cursor_visibility.on_update(delta);

		if (hp <= 0)//死亡时更新
			timer_die_effect_generation.on_update(delta);

		particle_list.erase(std::remove_if(//更新粒子列表中的对象
			particle_list.begin(), particle_list.end(),
			[](const Particle& particle)
			{
				return !particle.check_valid();
			}),
			particle_list.end());
		for (Particle& particle : particle_list)//遍历剩余粒子
			particle.on_update(delta);

		if (is_showing_sketch_frame)//判断是否需要播放剪影图片
			sketch_image(current_animation->get_frame(), &img_sketch);

		move_and_collide(delta);
	}

	virtual void on_draw(const Camera& camera)
	{
		if (is_jump_effect_visible)
			animation_jump_effect.on_draw(camera, (int)position_jump_effect.x, (int)position_jump_effect.y);
		if (is_land_effect_visible)
			animation_land_effect.on_draw(camera, (int)position_land_effect.x, (int)position_land_effect.y);

		for (const Particle& particle : particle_list)//粒子对象绘图方法
			particle.on_draw(camera);

		if (hp > 0 && is_invulnerable && is_showing_sketch_frame)//根据角色状态播放对应动画
			putimage_alpha(camera, (int)position.x, (int)position.y, &img_sketch);
		else
			current_animation->on_draw(camera, (int)position.x, (int)position.y);

		if (is_cursor_visible)
		{
			switch (id)
			{
			case PlayerID::P1:
				putimage_alpha(camera, (int)(position.x + (size.x - img_1P_cursor.getwidth()) / 2),
					(int)(position.y - img_1P_cursor.getheight()), &img_1P_cursor);
				break;
			case PlayerID::P2:
				putimage_alpha(camera, (int)(position.x + (size.x - img_2P_cursor.getwidth()) / 2),
					(int)(position.y - img_2P_cursor.getheight()), &img_2P_cursor);
				break;
			}
		}

		if (is_debug)//调试模式
		{
			setlinecolor(RGB(0, 125, 255));//绘制玩家碰撞矩形
			rectangle((int)position.x, (int)position.y, (int)(position.x + size.x), (int)(position.y + size.y));
		}
	}

	virtual void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN://按下按键使角色进入移动状态
			switch (id)//根据玩家对象切换键位 case
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
				case 0x41://A
					is_left_key_down = true;
					break;//隶属于按键A的 break

				case 0x44://D
					is_right_key_down = true;
					break;//隶属于按键D的 break

				case 0x57://W
					on_jump();
					break;//隶属于按键W的 break

				case 0x46://F
					if (can_attack)//检测是否可以普攻
					{
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();//重置定时器
					}
					break;

				case 0x47://G
					if (mp >= 100)
					{
						on_attack_ex();
						mp = 0;
					}
					break;
				}
				break;//玩家1对象 break

			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT://←
					is_left_key_down = true;
					break;//隶属于按键←的 break

				case VK_RIGHT://→
					is_right_key_down = true;
					break;//隶属于按键→的 break

				case VK_UP://↑
					on_jump();
					break;//隶属于按键↑的 break

				case VK_OEM_PERIOD://'.'
					if (can_attack)
					{
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();
					}
					break;

				case VK_OEM_2:// '/'
					if (mp >= 100)
					{
						on_attack_ex();
						mp = 0;
					}
					break;
				}
				break;//玩家2对象 break
			}
			break;//隶属于切换键位的 case的 break

		case WM_KEYUP://按键抬起使回到待机状态
			switch (id)//根据玩家对象切换键位 case
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
				case 0x41://A
					is_left_key_down = false;
					break;//隶属于按键A的 break

				case 0x44://D
					is_right_key_down = false;
					break;//隶属于按键D的 break
				}
				break;//玩家1对象 break
			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT://←
					is_left_key_down = false;
					break;//隶属于按键←的 break

				case VK_RIGHT://→
					is_right_key_down = false;
					break;//隶属于按键→的 break
				}
				break;//玩家2对象 break
			}
			break;//隶属于切换键位的 case的 break
		}
	}

	virtual void on_run(float distance)
	{
		if (is_attacking_ex)//判断是否在释放特殊攻击
			return;

		position.x += distance;//修改角色位置属性
		timer_run_effect_generation.resume();//重置跑动特效定时器

	}

	virtual void on_jump()
	{
		if (velocity.y != 0 || is_attacking_ex)//当玩家速度不等于0时不进行起跳逻辑//在特殊攻击时无法起跳
			return;//直接返回

		velocity.y += jump_velocity;//修改玩家跳跃速度
		is_jump_effect_visible = true;//设置跳跃特效可见
		animation_jump_effect.reset();//重置跳跃特效动画

		IMAGE* effect_frame = animation_jump_effect.get_frame();//获取跳跃特效动画播放位置
		position_jump_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
		position_jump_effect.y = position.y + size.y - effect_frame->getheight();
	}

	virtual void on_land()//落地逻辑
	{
		is_land_effect_visible = true;//设置落地特效可见
		animation_land_effect.reset();//重置落地特效动画

		IMAGE* effect_frame = animation_land_effect.get_frame();//获取落地特效动画播放位置
		position_land_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
		position_land_effect.y = position.y + size.y - effect_frame->getheight();
	}

	virtual void on_attack() {}
	virtual void on_attack_ex() {}

	void set_id(PlayerID id)
	{
		this->id = id;//玩家对象构造时对玩家ID赋值
	}

	void set_position(float x, float y)//初始化角色位置
	{
		position.x = x, position.y = y;
	}

	const Vector2& get_postion() const//获取角色位置
	{
		return position;
	}

	const Vector2& get_size() const//获取角色大小
	{
		return size;
	}

	void make_invulnerable()
	{
		is_invulnerable = true;//设置无敌状态
		timer_invulnerable.restart();//重置定时器
	}

	void set_hp(int val)
	{
		hp = val;
	}

	int get_hp() const
	{
		return hp;
	}

	int get_mp() const
	{
		return mp;
	}

protected:
	void move_and_collide(int delta)
	{
		float last_velocity_y = velocity.y;//上一帧玩家速度

		velocity.y += gravity * delta;//根据重力加速度的值对玩家速度进行累加
		position += velocity * (float)delta;//根据速度更新玩家位置

		if (hp <= 0)//防止角色被击飞后产生碰撞
			return;

		if (velocity.y > 0)//当y轴速度小于0时
		{
			for (const Platform& platform : platform_list)//遍历平台列表
			{
				const Platform::CollisionShape& shape = platform.shape;//获取平台外形
				bool is_collide_x = (max(position.x + size.x, shape.right) - min(position.x, shape.left) <= size.x + (shape.right - shape.left));//判断水平方向是否发生了碰撞
				bool is_collide_y = (shape.y >= position.y && shape.y <= position.y + size.y);////判断竖直方向是否发生了碰撞

				if (is_collide_x && is_collide_y)//两个方向都发生碰撞时
				{
					float delta_pos_y = velocity.y * delta;//计算y方向上移动的距离
					float last_tick_foot_pos_y = position.y + size.y - delta_pos_y;//计算上一帧玩家脚底的位置
					if (last_tick_foot_pos_y <= shape.y)//检测玩家是否整个穿过了平台
					{
						position.y = shape.y - size.y;//将玩家放置到平台上
						velocity.y = 0;//将y轴速度归0

						if (last_velocity_y != 0)//上一帧玩家速度不为0时
							on_land();//调用落地逻辑	

						break;
					}
				}
			}
		}

		if (!is_invulnerable)//当玩家不处于无敌状态时
		{
			for (Bullet* bullet : bullet_list)//遍历子弹对象
			{
				if (!bullet->get_valid() || bullet->get_collide_target() != id)//判断子弹是否可以发生碰撞//碰撞目标ID是否和当前碰撞玩家id匹配
					continue;

				if (bullet->check_collision(position, size))
				{
					make_invulnerable();
					bullet->on_collide();//调用对应子弹的碰撞代码
					bullet->set_valid(false);//设置为已经发生碰撞
					hp -= bullet->get_damage();//修改玩家生命值
					last_hurt_direction = bullet->get_position() - position;//根据受击方向和自动方向更新
					if (hp <= 0)//当生命值归零时
					{
						velocity.x = last_hurt_direction.x < 0 ? 0.35f : -0.35f;//设置速度
						velocity.y = -1.0f;//达到斜上抛运动的效果
					}
				}
			}
		}
	}

protected:
	const float gravity = 1.6e-3f;                  //重力设置
	const float run_velocity = 0.55f;               //跑动速度
	const float jump_velocity = -0.85f;             //跳跃速度

protected: //子类无法获取 privet成员

	int mp = 0;                                     //角色能量值
	int hp = 100;                                   //角色生命值

	Vector2 size;                                   //角色尺寸
	Vector2 position;                               //角色位置
	Vector2 velocity;                               //角色速度

	Animation animation_idle_left;                  //朝向左的默认动画
	Animation animation_idle_right;                 //朝向右的默认动画
	Animation animation_run_left;                   //朝向左的奔跑动画
	Animation animation_run_right;                  //朝向右的奔跑动画

	Animation animation_attack_ex_left;             //特殊攻击动画
	Animation animation_attack_ex_right;            //特殊攻击动画
	Animation animation_jump_effect;                //跳跃特效
	Animation animation_land_effect;                //落地特效

	Animation animation_die_left;					//朝左死亡动画
	Animation animation_die_right;					//朝右死亡动画

	bool is_jump_effect_visible = false;            //跳跃特效是否可见
	bool is_land_effect_visible = false;            //落地特效是否可见

	Vector2 position_jump_effect;                   //跳跃特效播放位置
	Vector2 position_land_effect;                   //落地特效播放位置

	Animation* current_animation = nullptr;         //当前播放的动画

	PlayerID id = PlayerID::P1;                     //玩家序号ID

	bool is_left_key_down = false;                  //记录玩家移动键盘是否被按下
	bool is_right_key_down = false;                 //记录玩家移动键盘是否被按下

	bool is_facing_right = true;                    //角色是否朝向右侧

	int attack_cd = 500;                            //普攻冷却
	bool can_attack = true;                         //是否可以释放普通攻击
	Timer timer_attack_cd;                          //普通攻击冷却时间

	bool is_attacking_ex = false;                   //是否在释放特殊攻击

	IMAGE img_sketch;                               //角色动画帧剪影图片
	bool is_invulnerable = false;                   //角色是否处于无敌状态
	bool is_showing_sketch_frame = false;           //当前帧是否显示剪影
	Timer timer_invulnerable;                       //无敌状态定时器
	Timer timer_invulnerable_blink;                 //无敌状态闪烁定时器

	Timer timer_run_effect_generation;              //跑动功能粒子发射定时器
	Timer timer_die_effect_generation;              //死亡特效粒子发射定时器

	bool is_cursor_visible = true;					//玩家光标指示器是否显示
	Timer timer_cursor_visibility;					//玩家光标指示器可见性定时器

	Vector2 last_hurt_direction;					//最后一次受击的方向

	std::vector<Particle> particle_list;            //粒子对象列表
};
