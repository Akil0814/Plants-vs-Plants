#pragma once
#include <cmath>

class Vector2//定义二维向量类
{
public:
	float x = 0;
	float y = 0;

public:
	Vector2() = default;
	~Vector2() = default;

	Vector2(float x, float y)//重载构造函数方便赋值
		: x(x), y(y) { }

	Vector2 operator+(const Vector2& vec) const
	{
		return Vector2(x + vec.x, y + vec.y);//当前对象 与 传入对象 的分量 相加 的结果//返回新的对象
											 //Vector2 v1(1, 2);
											 //Vector2 v2(3, 4);
											 //Vector2 v3 = v1 + v2; // v3 = (1+3, 2+4) = (4, 6)
	}

	void operator+=(const Vector2& vec)
	{
		x += vec.x, y += vec.y; //将 传入向量 的分量 加到 当前向量 对象上、
								//Vector2 v1(1, 2);
								//Vector2 v2(3, 4);
								//v1 += v2; // v1 变为 (1+3, 2+4) = (4, 6)
	}

	void operator-=(const Vector2& vec)
	{
		x -= vec.x, y -= vec.y;//将 传入向量 的分量从 当前向量 对象中 减去
							   //Vector2 v1(5, 6);
							   //Vector2 v2(3, 4);
							   //v1 -= v2; // v1 变为 (5-3, 6-4) = (2, 2)
	}

	Vector2 operator-(const Vector2& vec) const
	{
		return Vector2(x - vec.x, y - vec.y); //当前对象 与 传入对象 的分量 相减 的结果//返回新的对象
		                                      //Vector2 v1(5, 6);
											  //Vector2 v2(3, 4);
											  //Vector2 v3 = v1 - v2; // v3 = (5-3, 6-4) = (2, 2)
	}

	float operator*(const Vector2& vec) const
	{
		return x * vec.x + y * vec.y;//计算两个向量的点积（点积是一个标量值）
									 //Vector2 v1(1, 2); Vector2 v2(3, 4);
								     //float dotProduct = v1 * v2; // dotProduct = 1*3 + 2*4 = 11
	}

	Vector2 operator*(float val) const
	{
		return Vector2(x * val, y * val); //返回一个新对象 其分量是 当前对象 的分量乘以一个标量值
										  //Vector2 v(1, 2);
										  //Vector2 scaled = v * 3; // scaled = (1*3, 2*3) = (3, 6)
	}

	void operator*=(float val)
	{
		x *= val, y *= val;//将当前向量的分量直接乘以一个标量值
						   //Vector2 v(1, 2);
		                   //v *= 3; // v 变为 (1*3, 2*3) = (3, 6)
	}

	float length()//获取向量长度
	{
		return sqrt(x * x + y * y);//调用平方根函数
	}

	Vector2 normalize()//将向量标准化
	{
		float len = length();

		if (len == 0)
			return Vector2(0, 0); //如果向量长度为 0，返回零向量(0, 0)

		return Vector2(x / len, y / len);	//Vector2 v(3, 4);
											//Vector2 unit = v.normalize(); // unit = (3/5, 4/5) = (0.6, 0.8)
											//使其长度变为 1
	}
};