#pragma once
#include"atlas.h"
#include"util.h"
#include"camera.h"

#include<functional>
#include<graphics.h>

class Animation
{
public:
	Animation() = default;
	~Animation() = default;

	void reset()//���ö���״̬//���ü�ʱ���Ͷ���֡����
	{
		timer = 0;
		idx_frame = 0;
	}
	void set_atlas(Atlas* new_atlas)//���ö�����ʹ�õ�ͼ��
	{
		reset();//��ֹ֡����Խ�� �������ö���״̬
		atlas = new_atlas;
	}
	void set_loop(bool flag)//���ö����Ƿ�ѭ������
	{
		is_loop = flag;
	}
	void set_interval(int ms)//���ö���֡���
	{
		interval = ms;
	}
	int get_idx_frame()//��ȡ��ǰ֡����
	{
		return idx_frame;
	}
	IMAGE* get_frame()//��ȡ��ǰ֡ͼƬ
	{
		return atlas->get_image(idx_frame);
	}
	bool check_finishend()//��⶯���Ƿ����
	{
		if (is_loop)//��Ϊѭ������ ��Զ�������
			return false;

		return (idx_frame == atlas->get_size() - 1);//���֡���� �Ƿ񵽴���ͼ��β��
	}

	void on_update(int delta)//���¶���
	{
		timer += delta;//
		if (timer >= interval)//����ʱ������֡���ʱ
		{
			timer = 0;//���ü�ʱ��
			idx_frame++;//�л�����֡
			if (idx_frame >= atlas->get_size())//����֡���ﶯ��֡����ʱ
			{
				idx_frame = is_loop ? 0 : atlas->get_size() - 1;
				//����Ƿ�Ϊѭ��//�����Ƿ��ͷ��ʼ//���������һ��ͼƬ��

				//ǰ�����������֡��������ͼ��β��
				if (!is_loop && callback)//û��ѭ�������һص���������
					callback();//ִ�лص�����
			}
		}
	}

	void on_draw(const Camera& camera,int x,int y) const//��ͼ��Ա����//ʹ��const���η�ֹ�ں����ڲ��޸�����ֵ
	{
		putimage_alpha(camera,x, y, atlas->get_image(idx_frame));
	}

	void set_callback(std::function<void()>callback)
	{
		this->callback = callback;//������ĺ������󱣴浽�������callback��
	}

private:
	int timer = 0;//��ʱ��
	int interval = 0;//֡���
	int idx_frame = 0;//֡����
	bool is_loop = true;//�Ƿ�ѭ��
	Atlas* atlas = nullptr;//
	std::function<void()>callback;//function���͵ĺ�������
	//function��һ��ģ����//<>�ڵ�����Ϊ���溯������ֵΪvoid

};
