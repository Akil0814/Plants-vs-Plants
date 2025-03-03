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
		current_animation = &animation_idle_right;//����Ĭ�Ϸ���

		timer_attack_cd.set_wait_time(attack_cd);//��ʼ���չ���ȴ��ʱ��
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]()
			{
				can_attack = true;
			});

		timer_invulnerable.set_wait_time(750);//�����޵�ʱ��
		timer_invulnerable.set_one_shot(true);
		timer_invulnerable.set_callback([&]()
			{
				is_invulnerable = false;
			});

		timer_invulnerable_blink.set_wait_time(75);//�����޵�֡�л��ٶ�
		timer_invulnerable_blink.set_callback([&]()
			{
				is_showing_sketch_frame = !is_showing_sketch_frame;
			});

		timer_run_effect_generation.set_wait_time(75);//�ܶ�������Ч��ʱ��
		timer_run_effect_generation.set_callback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();//�������λ�ö�̬��������λ��
				particle_list.emplace_back(particle_position, &atlas_run_effect, 45);
			});

		timer_die_effect_generation.set_wait_time(35);//������Ч���Ӷ�ʱ��
		timer_die_effect_generation.set_callback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				particle_list.emplace_back(particle_position, &atlas_run_effect, 150);
			});

		animation_jump_effect.set_atlas(&atlas_jump_effect);//��Ծ��Ч����
		animation_jump_effect.set_interval(25);
		animation_jump_effect.set_loop(false);
		animation_jump_effect.set_callback([&]()
			{
				is_jump_effect_visible = false;
			});

		animation_land_effect.set_atlas(&atlas_land_effect);//�����Ч����
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
		int direction = is_right_key_down - is_left_key_down;//�����жϽ�ɫ����

		if (direction != 0)//�� direction��ֵ��Ϊ0ʱ
		{
			if (!is_attacking_ex)//û�н������⹥��ʱ
			   is_facing_right = direction > 0;//�޸�is_facing_right��ֵ
			current_animation = is_facing_right ? &animation_run_right : &animation_run_left;//������ҳ��򲥷Ų�ͬ����
			float distance = direction * run_velocity * delta;//����*�ܶ��ٶ�*ʱ��
			on_run(distance);//���ƶ����봫�� run����
		}
		else
		{
			current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;//û���ƶ�ʱѡ�񲥷ŵ�Ĭ�϶���
			timer_run_effect_generation.pause();//����ɫ�����ܶ�ʱֹͣ�ܶ����ӵ�����
		}

		if (is_attacking_ex)//���ݽ�ɫ���⹥��״̬
			current_animation = is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left;//�������Ŷ�Ӧ����Ķ���

		if (hp <= 0)//����ֵ����ʱ���Ŷ�Ӧ����
			current_animation = last_hurt_direction.x < 0 ? &animation_die_left : &animation_die_right;

		current_animation->on_update(delta);//�Ե�ǰ���ŵĶ������и���
		animation_jump_effect.on_update(delta);//������Ծ��Ч����
		animation_land_effect.on_update(delta);//���������Ч����

		timer_attack_cd.on_update(delta);//���¶�ʱ��
		timer_invulnerable.on_update(delta);
		timer_invulnerable_blink.on_update(delta);
		timer_run_effect_generation.on_update(delta);
		timer_cursor_visibility.on_update(delta);

		if (hp <= 0)//����ʱ����
			timer_die_effect_generation.on_update(delta);

		particle_list.erase(std::remove_if(//���������б��еĶ���
			particle_list.begin(), particle_list.end(),
			[](const Particle& particle)
			{
				return !particle.check_valid();
			}),
			particle_list.end());
		for (Particle& particle : particle_list)//����ʣ������
			particle.on_update(delta);

		if (is_showing_sketch_frame)//�ж��Ƿ���Ҫ���ż�ӰͼƬ
			sketch_image(current_animation->get_frame(), &img_sketch);

		move_and_collide(delta);
	}

	virtual void on_draw(const Camera& camera)
	{
		if (is_jump_effect_visible)
			animation_jump_effect.on_draw(camera, (int)position_jump_effect.x, (int)position_jump_effect.y);
		if (is_land_effect_visible)
			animation_land_effect.on_draw(camera, (int)position_land_effect.x, (int)position_land_effect.y);

		for (const Particle& particle : particle_list)//���Ӷ����ͼ����
			particle.on_draw(camera);

		if (hp > 0 && is_invulnerable && is_showing_sketch_frame)//���ݽ�ɫ״̬���Ŷ�Ӧ����
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

		if (is_debug)//����ģʽ
		{
			setlinecolor(RGB(0, 125, 255));//���������ײ����
			rectangle((int)position.x, (int)position.y, (int)(position.x + size.x), (int)(position.y + size.y));
		}
	}

	virtual void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN://���°���ʹ��ɫ�����ƶ�״̬
			switch (id)//������Ҷ����л���λ case
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
				case 0x41://A
					is_left_key_down = true;
					break;//�����ڰ���A�� break

				case 0x44://D
					is_right_key_down = true;
					break;//�����ڰ���D�� break

				case 0x57://W
					on_jump();
					break;//�����ڰ���W�� break

				case 0x46://F
					if (can_attack)//����Ƿ�����չ�
					{
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();//���ö�ʱ��
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
				break;//���1���� break

			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT://��
					is_left_key_down = true;
					break;//�����ڰ������� break

				case VK_RIGHT://��
					is_right_key_down = true;
					break;//�����ڰ������� break

				case VK_UP://��
					on_jump();
					break;//�����ڰ������� break

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
				break;//���2���� break
			}
			break;//�������л���λ�� case�� break

		case WM_KEYUP://����̧��ʹ�ص�����״̬
			switch (id)//������Ҷ����л���λ case
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
				case 0x41://A
					is_left_key_down = false;
					break;//�����ڰ���A�� break

				case 0x44://D
					is_right_key_down = false;
					break;//�����ڰ���D�� break
				}
				break;//���1���� break
			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT://��
					is_left_key_down = false;
					break;//�����ڰ������� break

				case VK_RIGHT://��
					is_right_key_down = false;
					break;//�����ڰ������� break
				}
				break;//���2���� break
			}
			break;//�������л���λ�� case�� break
		}
	}

	virtual void on_run(float distance)
	{
		if (is_attacking_ex)//�ж��Ƿ����ͷ����⹥��
			return;

		position.x += distance;//�޸Ľ�ɫλ������
		timer_run_effect_generation.resume();//�����ܶ���Ч��ʱ��

	}

	virtual void on_jump()
	{
		if (velocity.y != 0 || is_attacking_ex)//������ٶȲ�����0ʱ�����������߼�//�����⹥��ʱ�޷�����
			return;//ֱ�ӷ���

		velocity.y += jump_velocity;//�޸������Ծ�ٶ�
		is_jump_effect_visible = true;//������Ծ��Ч�ɼ�
		animation_jump_effect.reset();//������Ծ��Ч����

		IMAGE* effect_frame = animation_jump_effect.get_frame();//��ȡ��Ծ��Ч��������λ��
		position_jump_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
		position_jump_effect.y = position.y + size.y - effect_frame->getheight();
	}

	virtual void on_land()//����߼�
	{
		is_land_effect_visible = true;//���������Ч�ɼ�
		animation_land_effect.reset();//���������Ч����

		IMAGE* effect_frame = animation_land_effect.get_frame();//��ȡ�����Ч��������λ��
		position_land_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
		position_land_effect.y = position.y + size.y - effect_frame->getheight();
	}

	virtual void on_attack() {}
	virtual void on_attack_ex() {}

	void set_id(PlayerID id)
	{
		this->id = id;//��Ҷ�����ʱ�����ID��ֵ
	}

	void set_position(float x, float y)//��ʼ����ɫλ��
	{
		position.x = x, position.y = y;
	}

	const Vector2& get_postion() const//��ȡ��ɫλ��
	{
		return position;
	}

	const Vector2& get_size() const//��ȡ��ɫ��С
	{
		return size;
	}

	void make_invulnerable()
	{
		is_invulnerable = true;//�����޵�״̬
		timer_invulnerable.restart();//���ö�ʱ��
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
		float last_velocity_y = velocity.y;//��һ֡����ٶ�

		velocity.y += gravity * delta;//�����������ٶȵ�ֵ������ٶȽ����ۼ�
		position += velocity * (float)delta;//�����ٶȸ������λ��

		if (hp <= 0)//��ֹ��ɫ�����ɺ������ײ
			return;

		if (velocity.y > 0)//��y���ٶ�С��0ʱ
		{
			for (const Platform& platform : platform_list)//����ƽ̨�б�
			{
				const Platform::CollisionShape& shape = platform.shape;//��ȡƽ̨����
				bool is_collide_x = (max(position.x + size.x, shape.right) - min(position.x, shape.left) <= size.x + (shape.right - shape.left));//�ж�ˮƽ�����Ƿ�������ײ
				bool is_collide_y = (shape.y >= position.y && shape.y <= position.y + size.y);////�ж���ֱ�����Ƿ�������ײ

				if (is_collide_x && is_collide_y)//�������򶼷�����ײʱ
				{
					float delta_pos_y = velocity.y * delta;//����y�������ƶ��ľ���
					float last_tick_foot_pos_y = position.y + size.y - delta_pos_y;//������һ֡��ҽŵ׵�λ��
					if (last_tick_foot_pos_y <= shape.y)//�������Ƿ�����������ƽ̨
					{
						position.y = shape.y - size.y;//����ҷ��õ�ƽ̨��
						velocity.y = 0;//��y���ٶȹ�0

						if (last_velocity_y != 0)//��һ֡����ٶȲ�Ϊ0ʱ
							on_land();//��������߼�	

						break;
					}
				}
			}
		}

		if (!is_invulnerable)//����Ҳ������޵�״̬ʱ
		{
			for (Bullet* bullet : bullet_list)//�����ӵ�����
			{
				if (!bullet->get_valid() || bullet->get_collide_target() != id)//�ж��ӵ��Ƿ���Է�����ײ//��ײĿ��ID�Ƿ�͵�ǰ��ײ���idƥ��
					continue;

				if (bullet->check_collision(position, size))
				{
					make_invulnerable();
					bullet->on_collide();//���ö�Ӧ�ӵ�����ײ����
					bullet->set_valid(false);//����Ϊ�Ѿ�������ײ
					hp -= bullet->get_damage();//�޸��������ֵ
					last_hurt_direction = bullet->get_position() - position;//�����ܻ�������Զ��������
					if (hp <= 0)//������ֵ����ʱ
					{
						velocity.x = last_hurt_direction.x < 0 ? 0.35f : -0.35f;//�����ٶ�
						velocity.y = -1.0f;//�ﵽб�����˶���Ч��
					}
				}
			}
		}
	}

protected:
	const float gravity = 1.6e-3f;                  //��������
	const float run_velocity = 0.55f;               //�ܶ��ٶ�
	const float jump_velocity = -0.85f;             //��Ծ�ٶ�

protected: //�����޷���ȡ privet��Ա

	int mp = 0;                                     //��ɫ����ֵ
	int hp = 100;                                   //��ɫ����ֵ

	Vector2 size;                                   //��ɫ�ߴ�
	Vector2 position;                               //��ɫλ��
	Vector2 velocity;                               //��ɫ�ٶ�

	Animation animation_idle_left;                  //�������Ĭ�϶���
	Animation animation_idle_right;                 //�����ҵ�Ĭ�϶���
	Animation animation_run_left;                   //������ı��ܶ���
	Animation animation_run_right;                  //�����ҵı��ܶ���

	Animation animation_attack_ex_left;             //���⹥������
	Animation animation_attack_ex_right;            //���⹥������
	Animation animation_jump_effect;                //��Ծ��Ч
	Animation animation_land_effect;                //�����Ч

	Animation animation_die_left;					//������������
	Animation animation_die_right;					//������������

	bool is_jump_effect_visible = false;            //��Ծ��Ч�Ƿ�ɼ�
	bool is_land_effect_visible = false;            //�����Ч�Ƿ�ɼ�

	Vector2 position_jump_effect;                   //��Ծ��Ч����λ��
	Vector2 position_land_effect;                   //�����Ч����λ��

	Animation* current_animation = nullptr;         //��ǰ���ŵĶ���

	PlayerID id = PlayerID::P1;                     //������ID

	bool is_left_key_down = false;                  //��¼����ƶ������Ƿ񱻰���
	bool is_right_key_down = false;                 //��¼����ƶ������Ƿ񱻰���

	bool is_facing_right = true;                    //��ɫ�Ƿ����Ҳ�

	int attack_cd = 500;                            //�չ���ȴ
	bool can_attack = true;                         //�Ƿ�����ͷ���ͨ����
	Timer timer_attack_cd;                          //��ͨ������ȴʱ��

	bool is_attacking_ex = false;                   //�Ƿ����ͷ����⹥��

	IMAGE img_sketch;                               //��ɫ����֡��ӰͼƬ
	bool is_invulnerable = false;                   //��ɫ�Ƿ����޵�״̬
	bool is_showing_sketch_frame = false;           //��ǰ֡�Ƿ���ʾ��Ӱ
	Timer timer_invulnerable;                       //�޵�״̬��ʱ��
	Timer timer_invulnerable_blink;                 //�޵�״̬��˸��ʱ��

	Timer timer_run_effect_generation;              //�ܶ��������ӷ��䶨ʱ��
	Timer timer_die_effect_generation;              //������Ч���ӷ��䶨ʱ��

	bool is_cursor_visible = true;					//��ҹ��ָʾ���Ƿ���ʾ
	Timer timer_cursor_visibility;					//��ҹ��ָʾ���ɼ��Զ�ʱ��

	Vector2 last_hurt_direction;					//���һ���ܻ��ķ���

	std::vector<Particle> particle_list;            //���Ӷ����б�
};
