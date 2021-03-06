#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>
#include<conio.h>

#define High 30
#define Wideth 30
#define ENEMY_LIMIT 13
#define SPEED_LIMIT 10
#define SPEED_PRISE 4
#define REFRESH_FREQUENCY 70

int all[High][Wideth] = { 0 };
int enemy_x, enemy_y, prise_x, prise_y;
int all_enemy_number = 4, now_enemy_number = 0;
int position_x, position_y;
int enemy_speed = 0, prise_speed = 0;
int grades = 0;
int new_position_x, new_position_y;
int level_enemy_number = 15;
int level_enemy_speed = 0;
int level_fire = 0;

//移动光标的位置刷新图像
void goto_xy(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x,y };
	SetConsoleCursorPosition(handle, pos);
}
//去除光标的刷新图案
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1,0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
//设置控制台颜色
void color(int console)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, console|FOREGROUND_INTENSITY);
}
//爆炸道具闪屏特效
void color_boom()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
}
void start()
{
	int i, j;
	for (i = 0; i < High; i++)
	{
		all[i][0] = -1;
		all[i][Wideth - 1] = -1;
	}
	for (j = 0; j < Wideth; j++)
	{
		all[0][j] = -1;
		all[High - 1][j] = -1;
	}
	position_x = High / 2;
	position_y = Wideth / 2;
	all[position_x][position_y] = 1;//定位飞机的起始位置
	srand((unsigned)time(NULL));//初始化随机数种子
	enemy_x = 1;
	enemy_y = rand() % 25 + 1;
	all[enemy_x][enemy_y] = -2;
}
void show()
{
	goto_xy(0, 0);
	for (int i = 0; i < High; i++)
	{
		for (int j = 0; j < Wideth; j++)
		{
			if (all[i][j] == 0)
				printf(" ");
			else if (all[i][j] == -1)
				printf("#");
			else if (all[i][j] == 1)
				printf("A");
			else if (all[i][j] == -2)
				printf("@");
			else if (all[i][j] == 2)
				printf("|");
			else if (all[i][j] == 3)
				printf("S");
			else if (all[i][j] == 4)
				printf("B");
			else if (all[i][j] == 5)
				printf("M");
		}
		printf("\n");
	}
	printf("得分:%d", grades);
	Sleep(REFRESH_FREQUENCY);
}
//产生奖励道具模块
void creat_prise()
{
	prise_x = 1;
	prise_y = rand() % 25 + 1;
	all[prise_x][prise_y] = rand() % 3 + 3;
}
//爆炸道具实现模块
void boom()
{
	int count = 0;//计数此次击毁敌机数量
	for (int i = 0; i < High; i++)
		for (int j = 0; j < Wideth; j++)
			if (all[i][j] == -2)
				all[i][j] = 0,count++;
	printf("\a");
	color_boom();
	grades += count;
}
//道具移动模块
void prise_move()
{
	if (prise_speed < SPEED_PRISE)
		prise_speed++;
	else
	{
		for (int i = High - 1; i > 0; i--)
			for (int j = Wideth - 1; j > 0; j--)
				switch (all[i][j])
				{
				case 3:
					all[i][j] = 0;
					if (all[i + 1][j] == 0)
						all[i + 1][j] = 3;
					else if (all[i + 1][j] < 0)
						continue;
					else if (all[i + 1][j] == 1)
						level_fire++;
					break;
				case 4:
					all[i][j] = 0;
					if (all[i + 1][j] == 0)
						all[i + 1][j] = 4;
					else if (all[i + 1][j] < 0)
						continue;
					else if (all[i + 1][j] == 1)
						boom();
					break;
				case 5:
					all[i][j] = 0;
					if (all[i + 1][j] == 0)
						all[i + 1][j] = 5;
					else if (all[i + 1][j] < 0)
						continue;
					else if (all[i + 1][j] == 1)
						grades += 10;
					break;
				}
		prise_speed = 0;
	}
}
//武器等级模块
void fire_level()
{
	if (level_fire == 0)
		all[position_x - 1][position_y] = 2;
	else if (level_fire == 1)
	{
		if (all[position_x - 1][position_y - 1] != -1)
			all[position_x - 1][position_y - 1] = 2;
		if (all[position_x - 1][position_y + 1] != -1)
			all[position_x - 1][position_y + 1] = 2;
	}
	else if (level_fire == 2)
	{
		all[position_x - 1][position_y] = 2;
		if (all[position_x - 1][position_y - 1] != -1)
			all[position_x - 1][position_y - 1] = 2;
		if (all[position_x - 1][position_y + 1] != -1)
			all[position_x - 1][position_y + 1] = 2;
	}
	else if (level_fire >= 3)
	{
		if (all[position_x - 1][position_y - 1] != -1)
			all[position_x - 1][position_y - 1] = 2;
		if (all[position_x - 1][position_y + 1] != -1)
			all[position_x - 1][position_y + 1] = 2;
		if (all[position_x - 1][position_y - 2] != -1)
			all[position_x - 1][position_y - 2] = 2;
		if (all[position_x - 1][position_y + 2] != -1)
			all[position_x - 1][position_y + 2] = 2;
	}
	/*else if (level_fire >= 4)
	{
		all[position_x - 1][position_y] = 2;
		if (all[position_x - 1][position_y - 1] != -1)
			all[position_x - 1][position_y - 1] = 2;
		if (all[position_x - 1][position_y + 1] != -1)
			all[position_x - 1][position_y + 1] = 2;
	}*/
}
//武器更新模块
void fire()
{
	for (int i = 0; i < High; i++)
		for (int j = 0; j < Wideth; j++)
			if (all[i][j] == 2)
			{
				all[i][j] = 0;
				if (all[i - 1][j] == 0)
					all[i - 1][j] = 2;
				else if (all[i - 1][j] == -1)
					continue;
				else if (all[i - 1][j] == -2)
				{
					all[i - 1][j] = 0;
					grades++;
				}
			}
}
//敌人更新模块
void enemy()
{
	now_enemy_number = 0;
	for (int i = 0; i < High; i++)
		for (int j = 0; j < Wideth; j++)
			if (all[i][j] == -2)
				now_enemy_number++;

	if (enemy_speed < (SPEED_LIMIT - level_enemy_speed))
		enemy_speed++;
	else
	{
		for (int i = High - 1; i > 0; i--)
		{
			for (int j = Wideth - 1; j > 0; j--)
				if (all[i][j] == -2)
				{
					all[i][j] = 0;
					if (all[i + 1][j] == 2)
					{
						grades++;
						continue;
					}
					else if (all[i + 1][j] == 1)
					{
						goto_xy(0, 30);
						printf("游戏结束！总得分 %d\a\n", grades);
						Sleep(10000);
						exit(0);
					}
					else if (all[i + 1][j] == -1)
						continue;
					else if (all[i + 1][j] == 0)
						all[i + 1][j] = -2;
				}
		}
		enemy_speed = 0;
	}

	if (all_enemy_number < ENEMY_LIMIT&& (grades > level_enemy_number))
	{
		all_enemy_number++;
		level_enemy_number += 15;
		level_enemy_speed += 1;
		creat_prise();
	}
	else if(grades > level_enemy_number)
	{
		level_enemy_number += 15;
		creat_prise();
	}

	if ((now_enemy_number <= all_enemy_number) && (enemy_speed == 0))
	{
		enemy_y = rand() % 25 + 1;
		enemy_x = 1;
		all[enemy_x][enemy_y] = -2;
	}
}
//与用户无关的自更新模块
void withoutinput()
{
	system("color 0D");
	fire();
	enemy();
	prise_move();
}
//与用户有关的更新模块
void withinput()
{
	char input;

	if (_kbhit())
	{
		input = _getch();

		if (input == 'a')
		{
			new_position_x = position_x;
			new_position_y = position_y - 1;
		}
		else if (input == 'w')
		{
			new_position_x = position_x - 1;
			new_position_y = position_y;
		}
		else if (input == 's')
		{
			new_position_x = position_x + 1;
			new_position_y = position_y;
		}
		else if (input == 'd')
		{
			new_position_x = position_x;
			new_position_y = position_y + 1;
		}
		else if (input == ' ')
		{
			fire_level();
			new_position_x = position_x;
			new_position_y = position_y;
		}

		if (all[new_position_x][new_position_y] == -1)
			all[position_x][position_y] = 1;
		else if (all[new_position_x][new_position_y] == -2)
		{
			goto_xy(0, 30);
			printf("游戏结束！总得分 %d\a\n", grades);
			Sleep(10000);
			exit(0);
		}
		else if (all[new_position_x][new_position_y] == 3)
		{
			level_fire++;
			all[new_position_x][new_position_y] = 1;
		}
		else
		{
			all[position_x][position_y] = 0;
			all[new_position_x][new_position_y] = 1;
			position_x = new_position_x;
			position_y = new_position_y;
		}

	}
}
int main()
{
	start();
	color(13);
	while (1)
	{
		HideCursor();
		show();
		withoutinput();
		withinput();
	}
	return 0;
}