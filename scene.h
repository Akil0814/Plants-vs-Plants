#pragma once//��ֹͷ�ļ����ݱ��ظ���������������

#include"camera.h"
#include"animation.h"


#include<graphics.h>

class Scene//��������
{
public:
	Scene() = default;
	~Scene() = default;

	//��Ա����
	virtual void on_enter() {}//��������ʱ��ʼ���߼�
	virtual void on_update(int delta) {}//��������//delta������ʾʵ�ʹ�ȥ��ʱ��
	virtual void on_draw(const Camera& camera) {}//��Ⱦ��ͼ
	virtual void on_input(const ExMessage& msg) {}//�����������
	virtual void on_exit() {}//�����˳�ʱ��ж���߼�

private:

};