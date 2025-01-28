#pragma once
#include<functional>

class Timer
{
public:
	Timer()=default;
	~Timer()=default;

	void on_update(int delta)
	{
		if (paused)
			return;

		pass_time += delta;
		if (pass_time >= wait_time)
		{
			if ((!one_shot || (one_shot && !shotted)) && callback)//��ⶨʱ���Ƿ��ܱ�����
			{//�����ʱ�����ǵ��δ���//���߶�ʱΪ���δ�������û�б�������//ͬʱ���callback�Ƿ���Ч
				callback();//���ûص�����
			}
			shotted = true;//����Ϊ�Ѵ���
			pass_time = 0;//���öȹ���ʱ��
		}
	}

	void restart()//���ö�ʱ��״̬
	{
		pass_time = 0;//���¿�ʼ��ʱ
		shotted = false;//ֹͣ�¼��Ĵ���
	}

	void set_wait_time(int val)//���ö�ʱ���ȴ�ʱ��
	{
		wait_time = val;
	}

	void set_one_shot(bool flag)//�����Ƿ񵥴δ���
	{
		one_shot = flag;
	}

	void set_callback(std::function<void()>callback)//���ûص�����
	{
		this->callback = callback;
	}
	void pause()//��ʱ������ͣ
	{
		paused = true;
	}
	void resume()//��ʱ���ļ���
	{
		paused = false;
	}


private:
	int pass_time = 0;                //�ѹ�ʱ��
	int wait_time = 0;                //�ȴ�ʱ��
	bool paused = false;              //�Ƿ���ͣ
	bool shotted = false;             //�Ƿ񴥷�
	bool one_shot = false;            //�Ƿ񵥴δ���
	std::function<void()>callback;    //�����ص�
};
