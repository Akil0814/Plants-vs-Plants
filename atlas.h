#pragma once
#include<vector>
#include<graphics.h>

class Atlas
{
public:
	Atlas() = default;
	~Atlas() = default;

	void load_from_file(LPCTSTR path_template, int num)//����ͼƬ��Դ��Ա����
	{
		img_list.clear();//ʹ��ǰʹ��clear�����ͼƬ�����б�
		img_list.resize(num);//�����б����趨Ϊָ����С

		TCHAR path_file[256];
		for (int i = 0; i < num; i++)
		{
			_stprintf_s(path_file, path_template, i + 1);//ƴ���ļ�·��
			loadimage(&img_list[i], path_file);//�����ļ�
		}
	}

	void clear()//��ն���ͼ���б�
	{
		img_list.clear();
	}
	
	int get_size()//��ȡ����ͼ����ͼƬ����
	{
		return (int)img_list.size();
	}
	IMAGE* get_image(int idx)//���ڻ�ȡʵ����Ⱦ�Ķ���֡
	{
		if (idx < 0 || idx >= img_list.size())//���ͼ���Ƿ���Ч
			return nullptr;

		return &img_list[idx];//��Ч�򷵻ض�ӦͼƬ��ָ��
	}

	void add_image(const IMAGE& img)//��ͼ����������е�ͼƬ����
	{
		img_list.push_back(img);
	}

private:
	std::vector<IMAGE> img_list;//����ͼƬ�����б�
};