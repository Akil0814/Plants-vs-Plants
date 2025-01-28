#include"scene.h"
#include"menu_scene.h"
#include"game_scene.h"
#include"selector_scene.h"
#include"scene_manager.h"
#include"atlas.h"
#include"util.h"
#include"platform.h"
#include "player.h"
#include"bullet.h"

#include<graphics.h>

#pragma comment(lib, "Winmm.lib")//音频库文件链接

bool is_debug = false;//调试模式


//图片类型资源使用 img
//图集类型资源使用 atlas前缀
IMAGE img_menu_background;                        //主菜单背景

IMAGE img_VS;                                     //VS图标
IMAGE img_1P;                                     //1p文本图片
IMAGE img_2P;                                     //2p文本图片
IMAGE img_1P_desc;                                //1p键位描述
IMAGE img_2P_desc;                                //2p键位描述
IMAGE img_gravestone_left;                        //墓碑图片
IMAGE img_gravestone_right;                       //墓碑图片
IMAGE img_selector_tip;                           //角色选择界面提示信息文本图片
IMAGE img_selector_background;                    //角色选择界面背景
IMAGE img_1P_selector_btn_idle_left;              //1p向左选择按钮默认状态图片
IMAGE img_1P_selector_btn_idle_right;             //1p向右选择按钮默认状态图片
IMAGE img_1P_selector_btn_down_left;              //1p向左选择按钮按下状态图片
IMAGE img_1P_selector_btn_down_right;             //1p向右选择按钮按下状态图片
IMAGE img_2P_selector_btn_idle_left;              //2p向左选择按钮默认状态图片
IMAGE img_2P_selector_btn_idle_right;			  //2p向右选择按钮默认状态图片
IMAGE img_2P_selector_btn_down_left;	     	  //2p向左选择按钮按下状态图片
IMAGE img_2P_selector_btn_down_right;             //2p向右选择按钮按下状态图片
IMAGE img_peashooter_selector_background_left;    //角色选择界面向左豌豆背景图片
IMAGE img_peashooter_selector_background_right;   //角色选择界面向右豌豆背景图片
IMAGE img_sunflower_selector_background_left;     //角色选择界面向左向日葵背景图片
IMAGE img_sunflower_selector_background_right;    //角色选择界面向右向日葵背景图片

IMAGE img_sky;								      //天空
IMAGE img_hills;                                  //山脉
IMAGE img_platform_large;                         //大型平台图片
IMAGE img_platform_small;                         //小型平台图片

IMAGE img_1P_cursor;                              //1p指示光标图片
IMAGE img_2P_cursor;                              //2p指示光标图片

Atlas atlas_peashooter_idle_left;                 //婉逗射手朝向左的默认动画图集
Atlas atlas_peashooter_idle_right;                //婉逗射手朝向右的默认动画图集
Atlas atlas_peashooter_run_left;                  //婉逗射手朝向左的奔跑动画图集
Atlas atlas_peashooter_run_right;                 //婉逗射手朝向右的奔跑动画图集
Atlas atlas_peashooter_attack_ex_left;            //婉逗射手朝向左的特殊攻击动画图集
Atlas atlas_peashooter_attack_ex_right;           //婉逗射手朝向右的特殊攻击动画图集
Atlas atlas_peashooter_die_left;                  //婉逗射手朝向左的死亡动画图集
Atlas atlas_peashooter_die_right;                 //婉逗射手朝向右的死亡动画图集

Atlas atlas_sunflower_idle_left;                  //龙日葵朝向左的默认动画图集
Atlas atlas_sunflower_idle_right;                 //龙日葵朝向右的默认动画图集
Atlas atlas_sunflower_run_left;                   //龙日葵朝向左的奔跑动画图集
Atlas atlas_sunflower_run_right;                  //龙日葵朝向右的奔跑动画图集
Atlas atlas_sunflower_attack_ex_left;             //龙日葵朝向左的特殊攻击动画图集
Atlas atlas_sunflower_attack_ex_right;            //龙日葵朝向右的特殊攻击动画图集
Atlas atlas_sunflower_die_left;                   //龙日葵朝向左的死亡动画图集
Atlas atlas_sunflower_die_right;                  //龙日葵朝向右的死亡动画图集

IMAGE img_pea;                                    //豌豆图片
Atlas atlas_pea_break;                            //豌豆破碎动画图集
Atlas atlas_sun;                                  //日光动画图集
Atlas atlas_sun_explode;                          //日光爆炸动画图集
Atlas atlas_sun_ex;                               //特殊日光动画图集
Atlas atlas_sun_ex_explode;                       //特殊日光爆炸动画图集
Atlas atlas_sun_text;                             //“日”文本动画图集

Atlas atlas_run_effect;                           //奔跑特效图集
Atlas atlas_jump_effect;                          //跳跃特效图集
Atlas atlas_land_effect;                          //落地特效图集

IMAGE img_1P_winnner;                             //1p获胜文本图片
IMAGE img_2P_winnner;                             //2p获胜文本图片
IMAGE img_winnner_bar;                            //获胜玩家文本背景图片

IMAGE img_avatar_peashooter;                      //豌豆头像图片
IMAGE img_avatar_sunflower;                       //向日葵头像图片


Scene* menu_scene = nullptr;//Scene基类指针
Scene* game_scene = nullptr;//指向派生类的基类指针
Scene* selector_scene = nullptr;//在主循环内初始化

IMAGE* img_player_1_avatar = nullptr;//标记角色各自使用的头像图片素材
IMAGE* img_player_2_avatar = nullptr;

Camera main_camera;
SceneManager scene_manager;//实例化 scene_manager场景管理器//创建一个 scene_manager类

std::vector<Platform> platform_list;//平台列表
std::vector<Bullet*> bullet_list;//子弹列表//指针数组


Player* player_1 = nullptr;//定义两个玩家角色的指针
Player* player_2 = nullptr;

void flip_atlas(Atlas& src, Atlas& dst)//翻转图像
{
	dst.clear();//清空原有图集 防止重复调用时出现问题
	for (int i = 0; i < src.get_size(); i++)
	{
		IMAGE img_flpipped;//创建翻转图像对象
		flip_image(src.get_image(i), &img_flpipped);//对于每一帧图片进行翻转
		dst.add_image(img_flpipped);//使用 add image将翻转后的图片添加到目标图集中
	}
}

void load_game_resources()
{
	AddFontResourceEx(_T("resources/IPix.ttf"), FR_PRIVATE, NULL);//加载游戏字体

	loadimage(&img_menu_background, _T("resources/menu_background.png"));//加载并处理图片素材

	loadimage(&img_VS, _T("resources/VS.png"));
	loadimage(&img_1P, _T("resources/1P.png"));
	loadimage(&img_2P, _T("resources/2P.png"));
	loadimage(&img_1P_desc, _T("resources/1P_desc.png"));
	loadimage(&img_2P_desc, _T("resources/2P_desc.png"));
	loadimage(&img_gravestone_right, _T("resources/gravestone.png"));
	flip_image(&img_gravestone_right, &img_gravestone_left);
	loadimage(&img_selector_tip, _T("resources/selector_tip.png"));
	loadimage(&img_selector_background, _T("resources/selector_background.png"));
	loadimage(&img_1P_selector_btn_idle_right, _T("resources/1P_selector_btn_idle.png"));
	flip_image(&img_1P_selector_btn_idle_right, &img_1P_selector_btn_idle_left);
	loadimage(&img_1P_selector_btn_down_right, _T("resources/1P_selector_btn_down.png"));
	flip_image(&img_1P_selector_btn_down_right, &img_1P_selector_btn_down_left);
	loadimage(&img_2P_selector_btn_idle_right, _T("resources/2P_selector_btn_idle.png"));
	flip_image(&img_2P_selector_btn_idle_right, &img_2P_selector_btn_idle_left);
	loadimage(&img_2P_selector_btn_down_right, _T("resources/2P_selector_btn_down.png"));
	flip_image(&img_2P_selector_btn_down_right, &img_2P_selector_btn_down_left);
	loadimage(&img_peashooter_selector_background_right, _T("resources/peashooter_selector_background.png"));
	flip_image(&img_peashooter_selector_background_right, &img_peashooter_selector_background_left);
	loadimage(&img_sunflower_selector_background_right, _T("resources/sunflower_selector_background.png"));
	flip_image(&img_sunflower_selector_background_right, &img_sunflower_selector_background_left);

	loadimage(&img_sky, _T("resources/sky.png"));
	loadimage(&img_hills, _T("resources/hills.png"));
	loadimage(&img_platform_large, _T("resources/platform_large.png"));
	loadimage(&img_platform_small, _T("resources/platform_small.png"));

	loadimage(&img_1P_cursor, _T("resources/1P_cursor.png"));
	loadimage(&img_2P_cursor, _T("resources/2P_cursor.png"));

	atlas_peashooter_idle_right.load_from_file(_T("resources/peashooter_idle_%d.png"), 9);
	flip_atlas(atlas_peashooter_idle_right, atlas_peashooter_idle_left);
	atlas_peashooter_run_right.load_from_file(_T("resources/peashooter_run_%d.png"), 5);
	flip_atlas(atlas_peashooter_run_right, atlas_peashooter_run_left);
	atlas_peashooter_attack_ex_right.load_from_file(_T("resources/peashooter_attack_ex_%d.png"), 3);
	flip_atlas(atlas_peashooter_attack_ex_right, atlas_peashooter_attack_ex_left);
	atlas_peashooter_die_right.load_from_file(_T("resources/peashooter_die_%d.png"), 4);
	flip_atlas(atlas_peashooter_die_right, atlas_peashooter_die_left);

	atlas_sunflower_idle_right.load_from_file(_T("resources/sunflower_idle_%d.png"), 8);
	flip_atlas(atlas_sunflower_idle_right, atlas_sunflower_idle_left);
	atlas_sunflower_run_right.load_from_file(_T("resources/sunflower_run_%d.png"), 5);
	flip_atlas(atlas_sunflower_run_right, atlas_sunflower_run_left);
	atlas_sunflower_attack_ex_right.load_from_file(_T("resources/sunflower_attack_ex_%d.png"), 9);
	flip_atlas(atlas_sunflower_attack_ex_right, atlas_sunflower_attack_ex_left);
	atlas_sunflower_die_right.load_from_file(_T("resources/sunflower_die_%d.png"), 2);
	flip_atlas(atlas_sunflower_die_right, atlas_sunflower_die_left);

	loadimage(&img_pea, _T("resources/pea.png"));
	atlas_pea_break.load_from_file(_T("resources/pea_break_%d.png"), 3);
	atlas_sun.load_from_file(_T("resources/sun_%d.png"), 5);
	atlas_sun_explode.load_from_file(_T("resources/sun_explode_%d.png"), 5);
	atlas_sun_ex.load_from_file(_T("resources/sun_ex_%d.png"), 5);
	atlas_sun_ex_explode.load_from_file(_T("resources/sun_ex_explode_%d.png"), 5);
	atlas_sun_text.load_from_file(_T("resources/sun_text_%d.png"), 6);

	atlas_run_effect.load_from_file(_T("resources/run_effect_%d.png"), 4);
	atlas_jump_effect.load_from_file(_T("resources/jump_effect_%d.png"), 5);
	atlas_land_effect.load_from_file(_T("resources/land_effect_%d.png"), 2);

	loadimage(&img_1P_winnner, _T("resources/1P_winner.png"));
	loadimage(&img_2P_winnner, _T("resources/2P_winner.png"));
	loadimage(&img_winnner_bar, _T("resources/winnner_bar.png"));

	loadimage(&img_avatar_peashooter, _T("resources/avatar_peashooter.png"));
	loadimage(&img_avatar_sunflower, _T("resources/avatar_sunflower.png"));

	mciSendString(_T("open resources/bgm_game.mp3 alias bgm_game"), NULL, 0, NULL);//加载音乐音效
	mciSendString(_T("open resources/bgm_menu.mp3 alias bgm_menu"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_break_1.mp3 alias pea_break_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_break_2.mp3 alias pea_break_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_break_3.mp3 alias pea_break_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_shoot_1.mp3 alias pea_shoot_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_shoot_2.mp3 alias pea_shoot_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_shoot_ex.mp3 alias pea_shoot_ex"), NULL, 0, NULL);
	mciSendString(_T("open resources/sun_explode.mp3 alias sun_explode"), NULL, 0, NULL);
	mciSendString(_T("open resources/sun_explode_ex.mp3 alias sun_explode_ex"), NULL, 0, NULL);
	mciSendString(_T("open resources/sun_text.mp3 alias sun_text"), NULL, 0, NULL);
	mciSendString(_T("open resources/ui_confirm.wav alias ui_confirm"), NULL, 0, NULL);
	mciSendString(_T("open resources/ui_switch.wav alias ui_switch"), NULL, 0, NULL);
	mciSendString(_T("open resources/ui_win.wav alias ui_win"), NULL, 0, NULL);
}

int main()
{
	ExMessage msg;
	const int FPS = 60;
	
	load_game_resources();

	initgraph(1280, 720);

	settextstyle(28, 0, _T("IPix"));//载入字体名
	setbkmode(TRANSPARENT);//设置字体样式

	BeginBatchDraw();//在渲染缓冲区绘图

	menu_scene = new MenuScene();//初始化阶段实例化场景
	game_scene = new GameScene();//初始化阶段让基类指针指向派生类
	selector_scene=new SelectorScene();

	scene_manager.set_current_scene(menu_scene);

	while (true)
	{
		DWORD frame_start_time = GetTickCount();//开始——程序运行到此处消耗的毫秒数

		while (peekmessage(&msg))
		{
			scene_manager.on_input(msg);//处理对应场景玩家输入
		}

		static DWORD last_tick_time = GetTickCount();//使用静态变量
		DWORD current_tick_time = GetTickCount();
		DWORD delta_tick = current_tick_time - last_tick_time;//计算上次调用on update和这次的时间间隔
		scene_manager.on_update(delta_tick);//处理对应场景逻辑
		last_tick_time = current_tick_time;

		cleardevice();//清空窗口
		scene_manager.on_draw(main_camera);//绘制对应场景图片素材
		FlushBatchDraw();//显示渲染缓冲区的内容

		DWORD frame_end_time = GetTickCount();//结束——程序运行到此处消耗的毫秒数
		DWORD frame_delta_time = frame_end_time - frame_start_time;//通过做差计算时间循环消耗的秒数
		if (frame_delta_time < 1000 / FPS)//1000表示一秒//当消耗时间小于期望渲染时间时
			Sleep(1000 / FPS - frame_delta_time);//使程序暂停一段时间
	}

	EndBatchDraw();//关闭渲染缓冲区

	
	return 0;
}