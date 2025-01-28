#pragma once//防止头文件内容被重复包含产生二义性

#include"camera.h"
#include"animation.h"


#include<graphics.h>

class Scene//场景基类
{
public:
	Scene() = default;
	~Scene() = default;

	//成员方法
	virtual void on_enter() {}//场景进入时初始化逻辑
	virtual void on_update(int delta) {}//处理数据//delta参数表示实际过去的时间
	virtual void on_draw(const Camera& camera) {}//渲染绘图
	virtual void on_input(const ExMessage& msg) {}//处理玩家输入
	virtual void on_exit() {}//场景退出时的卸载逻辑

private:

};