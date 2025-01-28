#pragma once
#include <cmath>

class Vector2//�����ά������
{
public:
	float x = 0;
	float y = 0;

public:
	Vector2() = default;
	~Vector2() = default;

	Vector2(float x, float y)//���ع��캯�����㸳ֵ
		: x(x), y(y) { }

	Vector2 operator+(const Vector2& vec) const
	{
		return Vector2(x + vec.x, y + vec.y);//��ǰ���� �� ������� �ķ��� ��� �Ľ��//�����µĶ���
											 //Vector2 v1(1, 2);
											 //Vector2 v2(3, 4);
											 //Vector2 v3 = v1 + v2; // v3 = (1+3, 2+4) = (4, 6)
	}

	void operator+=(const Vector2& vec)
	{
		x += vec.x, y += vec.y; //�� �������� �ķ��� �ӵ� ��ǰ���� �����ϡ�
								//Vector2 v1(1, 2);
								//Vector2 v2(3, 4);
								//v1 += v2; // v1 ��Ϊ (1+3, 2+4) = (4, 6)
	}

	void operator-=(const Vector2& vec)
	{
		x -= vec.x, y -= vec.y;//�� �������� �ķ����� ��ǰ���� ������ ��ȥ
							   //Vector2 v1(5, 6);
							   //Vector2 v2(3, 4);
							   //v1 -= v2; // v1 ��Ϊ (5-3, 6-4) = (2, 2)
	}

	Vector2 operator-(const Vector2& vec) const
	{
		return Vector2(x - vec.x, y - vec.y); //��ǰ���� �� ������� �ķ��� ��� �Ľ��//�����µĶ���
		                                      //Vector2 v1(5, 6);
											  //Vector2 v2(3, 4);
											  //Vector2 v3 = v1 - v2; // v3 = (5-3, 6-4) = (2, 2)
	}

	float operator*(const Vector2& vec) const
	{
		return x * vec.x + y * vec.y;//�������������ĵ���������һ������ֵ��
									 //Vector2 v1(1, 2); Vector2 v2(3, 4);
								     //float dotProduct = v1 * v2; // dotProduct = 1*3 + 2*4 = 11
	}

	Vector2 operator*(float val) const
	{
		return Vector2(x * val, y * val); //����һ���¶��� ������� ��ǰ���� �ķ�������һ������ֵ
										  //Vector2 v(1, 2);
										  //Vector2 scaled = v * 3; // scaled = (1*3, 2*3) = (3, 6)
	}

	void operator*=(float val)
	{
		x *= val, y *= val;//����ǰ�����ķ���ֱ�ӳ���һ������ֵ
						   //Vector2 v(1, 2);
		                   //v *= 3; // v ��Ϊ (1*3, 2*3) = (3, 6)
	}

	float length()//��ȡ��������
	{
		return sqrt(x * x + y * y);//����ƽ��������
	}

	Vector2 normalize()//��������׼��
	{
		float len = length();

		if (len == 0)
			return Vector2(0, 0); //�����������Ϊ 0������������(0, 0)

		return Vector2(x / len, y / len);	//Vector2 v(3, 4);
											//Vector2 unit = v.normalize(); // unit = (3/5, 4/5) = (0.6, 0.8)
											//ʹ�䳤�ȱ�Ϊ 1
	}
};