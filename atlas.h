#pragma once
#include<vector>
#include<graphics.h>

class Atlas
{
public:
	Atlas() = default;
	~Atlas() = default;

	void load_from_file(LPCTSTR path_template, int num)//加载图片资源成员方法
	{
		img_list.clear();//使用前使用clear清空了图片对象列表
		img_list.resize(num);//并将列表长度设定为指定大小

		TCHAR path_file[256];
		for (int i = 0; i < num; i++)
		{
			_stprintf_s(path_file, path_template, i + 1);//拼凑文件路径
			loadimage(&img_list[i], path_file);//载入文件
		}
	}

	void clear()//清空对象图像列表
	{
		img_list.clear();
	}
	
	int get_size()//获取对象图集中图片数量
	{
		return (int)img_list.size();
	}
	IMAGE* get_image(int idx)//用于获取实际渲染的动画帧
	{
		if (idx < 0 || idx >= img_list.size())//检测图集是否有效
			return nullptr;

		return &img_list[idx];//有效则返回对应图片的指针
	}

	void add_image(const IMAGE& img)//像图集中添加已有的图片对象
	{
		img_list.push_back(img);
	}

private:
	std::vector<IMAGE> img_list;//定义图片对象列表
};