#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<memory.h>
#include<stdlib.h>
#include<time.h>

// board information
#define BOARD_SIZE 12
#define EMPTY 0
#define BLACK 1
#define WHITE 2

// bool
typedef int BOOL;
#define TRUE 1
#define FALSE 0

// option
typedef int OPTION;
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define UP_LEFT 4
#define UP_RIGHT 5
#define DOWN_LEFT 6
#define DOWN_RIGHT 7

#define MAX_BYTE 10000

#define START "START"
#define PLACE "PLACE"
#define TURN  "TURN"
#define END   "END"

struct Command
{
	int x;
	int y;
	OPTION option;
};

//记录黑白棋子位置
struct meplace
{
	int x;
	int y;
};
typedef struct meplace Mplace;




char buffer[MAX_BYTE] = { 0 };
char board[BOARD_SIZE][BOARD_SIZE] = { 0 };

int me_flag;
int other_flag;

const int DIR[8][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };

void debug(const char *str)
{
	printf("DEBUG %s\n", str);
	fflush(stdout);
}

void printBoard() {
	char visual_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == EMPTY) {
				visual_board[i][j] = '.';
			}
			else if (board[i][j] == BLACK) {
				visual_board[i][j] = 'O';
			}
			else if (board[i][j] == WHITE) {
				visual_board[i][j] = 'X';
			}
		}
		printf("%s\n", visual_board[i]);
	}
}

BOOL isInBound(int x, int y) {
	return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

/**
* YOUR CODE BEGIN
* 你的代码开始
*/

/**
* You can define your own struct and variable here
* 你可以在这里定义你自己的结构体和变量
*/
int VAL = 0;

Mplace flag[3][16];


/**
* 你可以在这里初始化你的AI
*/

void initAI(int me) {

}





int evaluate(char myboard[BOARD_SIZE][BOARD_SIZE], Mplace myflag[3][16])
{

	int value = 0;
	int num = 0;
	for (int p = 0; p < 16; p++)
	{
		//数子估值
		if (myflag[me_flag][p].x == -1 || myflag[me_flag][p].y == -1) continue;
		else
		{
			value += 10000;
			num++;
		}
		//凝聚估值
		if (myflag[me_flag][p].x == 4 && myflag[me_flag][p].y >= 4 && myflag[me_flag][p].y <= 7) value += 300;
		if (myflag[me_flag][p].x == 5 && myflag[me_flag][p].y >= 4 && myflag[me_flag][p].y <= 7) value += 500;
		if (myflag[me_flag][p].x == 6 && myflag[me_flag][p].y >= 4 && myflag[me_flag][p].y <= 7) value += 500;
		if (myflag[me_flag][p].x == 7 && myflag[me_flag][p].y >= 4 && myflag[me_flag][p].y <= 7) value += 300;
		if (myflag[me_flag][p].x >= 10) value -= 600;
		if (myflag[me_flag][p].x <= 1) value -= 600;
		if (myflag[me_flag][p].y >= 10) value -= 600;
		if (myflag[me_flag][p].y <= 1) value -= 600;

		if (myflag[me_flag][p].x == 9) value -= 300;
		if (myflag[me_flag][p].x == 2) value -= 300;
		if (myflag[me_flag][p].y == 9) value -= 300;
		if (myflag[me_flag][p].y == 2) value -= 300;

		//十字形估值
		if (myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y] == me_flag&&myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y] == me_flag) value += 200;
		if (myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 1] == me_flag&&myboard[myflag[me_flag][p].x][myflag[me_flag][p].y - 1] == me_flag) value += 200;

		//类八皇后位置估值
		if (myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y + 2] == me_flag) value += 200;
		if (myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y - 2] == me_flag) value += 200;
		if (myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y - 2] == me_flag) value += 200;
		if (myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y + 2] == me_flag) value += 200;


	}

	/////防守估值（被弃用）
	/*
	if (myflag[me_flag][p].x != -1 && myflag[me_flag][p].y != -1 && myflag[me_flag][p].x + 2 < 12 && myflag[me_flag][p].x - 2 >= 0 && myflag[me_flag][p].y + 2 < 12 && myflag[me_flag][p].y - 2 >= 0)
	{

	//夹（对方夹我方）
	if (myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y - 1] == other_flag &&myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y + 1] == EMPTY && (myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 2] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y + 2] == other_flag || myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y + 2] == other_flag || myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y] == other_flag)) value -= 1400;//
	if (myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y + 1] == other_flag &&myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y - 1] == EMPTY && (myboard[myflag[me_flag][p].x][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y - 2] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y - 2] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y - 2] == other_flag)) value -= 1400;//
	if (myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y + 1] == other_flag &&myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y - 1] == EMPTY && (myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y - 2] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y - 2] == other_flag || myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y - 2] == other_flag)) value -= 1400;//
	if (myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y - 1] == other_flag &&myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y + 1] == EMPTY && (myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y + 2] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y + 2] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 2] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y] == other_flag)) value -= 1400;//敌子与我一子成斜线

	if (myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y] == other_flag && myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y] == EMPTY && (myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 1] == other_flag)) value -= 1400;//
	if (myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 1] == other_flag && myboard[myflag[me_flag][p].x][myflag[me_flag][p].y - 1] == EMPTY && (myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y - 2] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y] == other_flag)) value -= 1400;//
	if (myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y] == other_flag && myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y] == EMPTY && (myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y - 1] == other_flag)) value -= 1400;//
	if (myboard[myflag[me_flag][p].x][myflag[me_flag][p].y - 1] == other_flag && myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 1] == EMPTY && (myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y + 2] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 2] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y + 2] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y] == other_flag)) value -= 1400;//敌子与我一子成直线

	//挑（对方挑我方）

	if (myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y + 2] == me_flag&&myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y + 1] == EMPTY && (myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 2] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y + 2] == other_flag || myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y] == other_flag)) { value -= 2900; break; }
	if (myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y - 2] == me_flag&&myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y - 1] == EMPTY && (myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y - 2] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y - 2] == other_flag)) { value -= 2900; break; }

	if (myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y] == me_flag&&myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y] == EMPTY && (myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y - 1] == other_flag)) { value -= 2900; break; }
	if (myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y] == me_flag&&myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y] == EMPTY && (myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y - 1] == other_flag)) { value -= 2900; break; }

	if (myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y - 2] == me_flag&&myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y - 1] == EMPTY && (myboard[myflag[me_flag][p].x][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x + 2][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y - 2] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y - 2] == other_flag)) { value -= 2900; break; }
	if (myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y + 2] == me_flag&&myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y + 1] == EMPTY && (myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y + 2] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 2] == other_flag || myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x - 2][myflag[me_flag][p].y] == other_flag)) { value -= 2900; break; }

	if (myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 2] == me_flag&&myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 1] == EMPTY && (myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y + 2] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y + 2] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y + 1] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y] == other_flag)) { value -= 2900; break; }
	if (myboard[myflag[me_flag][p].x][myflag[me_flag][p].y - 2] == me_flag&&myboard[myflag[me_flag][p].x][myflag[me_flag][p].y - 1] == EMPTY && (myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y - 2] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y - 2] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y - 1] == other_flag || myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y] == other_flag)) { value -= 2900; break; }

	//if (myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y] == me_flag&&myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 1] == me_flag&&myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y + 1] == me_flag&&myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y] == me_flag&&myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y - 1] == me_flag&&myboard[myflag[me_flag][p].x + 1][myflag[me_flag][p].y - 1] == me_flag&&myboard[myflag[me_flag][p].x - 1][myflag[me_flag][p].y + 1] == me_flag&&myboard[myflag[me_flag][p].x][myflag[me_flag][p].y + 1] == me_flag) value += 1500;


	//if (myboard[x + 1][y] == EMPTY&&myboard[x - 1][y] == EMPTY&&myboard[x - 1][y + 1] == EMPTY&&myboard[x - 1][y - 1] == EMPTY&&myboard[x + 1][y - 1] == EMPTY&&myboard[x + 1][y + 1] == EMPTY&&myboard[x][y - 1] == EMPTY&&myboard[x][y + 1] == EMPTY) value -= 1000;

	}
	}
	}
	}
	*/
	//Mplace nowflag[3][16] = { -1 };//////另一种版本的进攻函数最终被弃用
	/*
	for (int q = 0; q < 16; ++q)
	{
	if (myflag[other_flag][q].x != -1 && myflag[other_flag][q].y != -1 && myflag[other_flag][q].x + 2 < 12 && myflag[other_flag][q].x - 2 >= 0 && myflag[other_flag][q].y + 2 < 12 && myflag[other_flag][q].y - 2 >= 0)
	{
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 1] == EMPTY)
	{
	if (myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y + 2] == other_flag)
	{
	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag)
	{
	value += 3000;

	}
	}

	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 1] == me_flag)
	{

	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 2] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 2] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y + 2] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y + 1] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag) value += 1500;
	}
	}
	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 1] == EMPTY)
	{
	if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y - 2] == other_flag)
	{
	if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 2] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 2] == me_flag)
	{
	value += 3000;

	}
	}
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 1] == me_flag)
	{
	if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y - 1] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 2] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 2] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y - 2] == me_flag) value += 1500;
	}
	}

	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 1] == EMPTY)
	{
	if (myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y - 2] == other_flag)
	{
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 1] == EMPTY && (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 2] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 2] == me_flag))
	{
	value += 3000;

	}
	}
	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 1] == me_flag)
	{
	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y - 1] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y - 2] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 2] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 2] == me_flag) value += 1500;
	}
	}


	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 1] == EMPTY)
	{
	if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y + 2] == other_flag)
	{
	if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y] == me_flag)
	{
	value += 3000;

	}
	}
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 1] == me_flag)
	{
	if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y + 1] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y + 2] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 2] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 2] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y] == me_flag) value += 1500;

	}	//敌子与我一子成斜线
	}
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == EMPTY)
	{

	if (myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y] == other_flag)
	{
	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag)
	{
	value += 3000;

	}
	}

	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag)
	{
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 1] == me_flag) value += 1500;//
	if (myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y + 1] == me_flag) value += 1500;//
	if (myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y] == me_flag) value += 1500;//
	if (myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y - 1] == me_flag) value += 1500;//
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 1] == me_flag) value += 1500;//
	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag) value += 1500;//
	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag) value += 1500;//

	}
	}
	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == EMPTY)
	{

	if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y] == other_flag)
	{
	if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y - 1] == me_flag)
	{
	value += 3000;

	}
	}

	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag)
	{
	if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y] == me_flag) value += 1500;//
	if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y + 1] == me_flag) value += 1500;//
	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 1] == me_flag) value += 1500;//
	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag) value += 1500;//
	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag) value += 1500;//
	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 1] == me_flag) value += 1500;//
	if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y - 1] == me_flag) value += 1500;//
	}
	}
	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == EMPTY)
	{

	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 2] == other_flag)
	{
	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag)
	{
	value += 3000;

	}
	}

	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag)
	{
	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 1] == me_flag) value += 1500;//敌子与我一子成直线
	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 2] == me_flag) value += 1500;//敌子与我一子成直线
	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 2] == me_flag) value += 1500;//敌子与我一子成直线
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 2] == me_flag) value += 1500;//敌子与我一子成直线
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 1] == me_flag) value += 1500;//敌子与我一子成直线
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag) value += 1500;//敌子与我一子成直线
	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag) value += 1500;//敌子与我一子成直线
	}
	}
	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == EMPTY)
	{

	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 2] == other_flag)
	{
	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 2] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 2] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag)
	{
	value += 3000;

	}
	}

	if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag)
	{
	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 1] == me_flag) value += 1500;
	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag) value += 1500;//
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag) value += 1500;//
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 1] == me_flag) value += 1500;//
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 2] == me_flag) value += 1500;//
	if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag) value += 1500;//
	if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag) value += 1500;//
	}
	}
	}
	}
	*/


	for (int q = 0; q < 16; ++q)//进攻估值（3000为挑位 1500为夹位）
	{
		if (myflag[other_flag][q].x != -1 && myflag[other_flag][q].y != -1 && myflag[other_flag][q].x + 2 < 12 && myflag[other_flag][q].x - 2 >= 0 && myflag[other_flag][q].y + 2 < 12 && myflag[other_flag][q].y - 2 >= 0)
		{
			if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 1] == EMPTY)
			{
				if (myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y + 2] == other_flag)
				{
					if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag)
					{
						value += 3000;
					}
				}
				if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 1] == me_flag && (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag))
				{
					value += 1500;
				}
			}
			if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 1] == EMPTY)
			{
				if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y - 2] == other_flag)
				{
					if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 2] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 2] == me_flag)
					{
						value += 3000;
					}
				}
				if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 1] == me_flag && (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 2] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 2] == me_flag || myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y - 2] == me_flag))
					value += 1500;
			}

			if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 1] == EMPTY)
			{
				if (myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y - 2] == other_flag)
				{
					if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 1] == EMPTY && (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 2] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 2] == me_flag))
					{
						value += 3000;
					}
				}
				if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 1] == me_flag && (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y - 2] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 2] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 2] == me_flag))
					value += 1500;//
			}

			if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 1] == EMPTY)
			{
				if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y + 2] == other_flag)
				{
					if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y] == me_flag)
					{
						value += 3000;
					}
				}
				if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 1] == me_flag && (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y] == me_flag))
					value += 1500;//敌子与我一子成斜线
			}
			if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == EMPTY)
			{

				if (myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y] == other_flag)
				{
					if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag)
					{
						value += 3000;
					}
				}

				if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag && (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x + 2][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag))
					value += 1500;//
			}
			if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == EMPTY)
			{

				if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y] == other_flag)
				{
					if (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y - 1] == me_flag)
					{
						value += 3000;
					}
				}

				if (myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag && (myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x - 2][myflag[other_flag][q].y - 1] == me_flag))
					value += 1500;//
			}
			if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == EMPTY)
			{

				if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 2] == other_flag)
				{
					if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag)
					{
						value += 3000;
					}
				}

				if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == me_flag && (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 2] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y + 1] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag))
					value += 1500;//敌子与我一子成直线
			}
			if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 1] == EMPTY)
			{

				if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y - 2] == other_flag)
				{
					if (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 2] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 2] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag)
					{
						value += 3000;
					}
				}

				if (myboard[myflag[other_flag][q].x][myflag[other_flag][q].y + 1] == me_flag && (myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 1] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y - 2] == me_flag || myboard[myflag[other_flag][q].x - 1][myflag[other_flag][q].y] == me_flag || myboard[myflag[other_flag][q].x + 1][myflag[other_flag][q].y] == me_flag))
					value += 1500;//
			}
		}
	}


	return value;
}



BOOL myplace(char moveboard[12][12], int x, int y, OPTION option, int cur_flag, Mplace FLAG[3][16])//模拟棋盘时专用place 函数（内含存黑白子操作）

{
	int cur_other_flag = 3 - cur_flag;

	if (moveboard[x][y] != cur_flag)
	{
		return FALSE;
	}
	int new_x = x + DIR[option][0];
	int new_y = y + DIR[option][1];
	// 移动之后的位置超出边界, 或者不是空地
	if (!isInBound(new_x, new_y) || moveboard[new_x][new_y] != EMPTY)
	{
		return FALSE;
	}
	for (int i = 0; i < 16; ++i)
	{
		if (FLAG[cur_flag][i].x == x&&FLAG[cur_flag][i].y == y)
		{
			FLAG[cur_flag][i].x = new_x;
			FLAG[cur_flag][i].y = new_y;
			break;
		}
	}

	moveboard[x][y] = EMPTY;
	moveboard[new_x][new_y] = cur_flag;
	// 挑
	int intervention_dir[4][2] = { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 1, -1 } };
	for (int i = 0; i < 4; i++) {
		int x1 = new_x + intervention_dir[i][0];
		int y1 = new_y + intervention_dir[i][1];
		int x2 = new_x - intervention_dir[i][0];
		int y2 = new_y - intervention_dir[i][1];
		if (isInBound(x1, y1) && isInBound(x2, y2) && moveboard[x1][y1] == cur_other_flag &&moveboard[x2][y2] == cur_other_flag)
		{
			moveboard[x1][y1] = cur_flag;
			moveboard[x2][y2] = cur_flag;
			for (int i = 0; i < 16; ++i)
			{
				if (FLAG[cur_other_flag][i].x == x1&&FLAG[cur_other_flag][i].y == y1)
				{
					FLAG[cur_other_flag][i].x = -1;
					FLAG[cur_other_flag][i].y = -1;
					break;
				}
			}
			for (int i = 0; i < 16; ++i)
			{
				if (FLAG[cur_other_flag][i].x == x2&&FLAG[cur_other_flag][i].y == y2)
				{
					FLAG[cur_other_flag][i].x = -1;
					FLAG[cur_other_flag][i].y = -1;
					break;
				}
			}
			for (int i = 0; i < 16; ++i)
			{
				if (FLAG[cur_flag][i].x == -1 && FLAG[cur_flag][i].y == -1)
				{
					FLAG[cur_flag][i].x = x1;
					FLAG[cur_flag][i].y = y1;
					break;
				}
			}
			for (int i = 0; i < 16; ++i)
			{
				if (FLAG[cur_flag][i].x == -1 && FLAG[cur_flag][i].y == -1)
				{
					FLAG[cur_flag][i].x = x2;
					FLAG[cur_flag][i].y = y2;
					break;
				}
			}

		}
	}
	// 夹
	int custodian_dir[8][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } };
	for (int i = 0; i < 8; i++) {
		int x1 = new_x + custodian_dir[i][0];
		int y1 = new_y + custodian_dir[i][1];
		int x2 = new_x + custodian_dir[i][0] * 2;
		int y2 = new_y + custodian_dir[i][1] * 2;
		if (isInBound(x1, y1) && isInBound(x2, y2) && moveboard[x2][y2] == cur_flag && moveboard[x1][y1] == cur_other_flag)
		{
			moveboard[x1][y1] = cur_flag;
			for (int i = 0; i < 16; ++i)
			{
				if (FLAG[cur_other_flag][i].x == x1&&FLAG[cur_other_flag][i].y == y1)
				{
					FLAG[cur_other_flag][i].x = -1;
					FLAG[cur_other_flag][i].y = -1;
					break;
				}
			}
			for (int i = 0; i < 16; ++i)
			{
				if (FLAG[cur_flag][i].x == -1 && FLAG[cur_flag][i].y == -1)
				{
					FLAG[cur_flag][i].x = x1;
					FLAG[cur_flag][i].y = y1;
					break;
				}
			}
		}
	}
	return TRUE;
}


int Alpha_Beta(char myboard[BOARD_SIZE][BOARD_SIZE], int depth, int alpha, int beta, int cur_flag, Mplace myflag[][16])//Alpha_beta
{
	if (depth == 0)
	{
		return evaluate(myboard, myflag);
	}
	char nowboard[BOARD_SIZE][BOARD_SIZE] = { 0 };
	Mplace nowflag[3][16];
	int cur_other_flag = 3 - cur_flag;
	for (int i = 0; i < 16; ++i)
	{
		if (myflag[cur_flag][i].x == -1 || myflag[cur_flag][i].y == -1) continue;
		for (int k = 0; k < 8; k++)
		{
			if (myboard[myflag[cur_flag][i].x + DIR[k][0]][myflag[cur_flag][i].y + DIR[k][1]] != EMPTY) continue;
			if (myflag[cur_flag][i].x + DIR[k][0]>11 || myflag[cur_flag][i].x + DIR[k][0]<0 || myflag[cur_flag][i].y + DIR[k][1]>11 || myflag[cur_flag][i].y + DIR[k][1] < 0) continue;
			{
				memcpy(nowboard, myboard, sizeof(board));//复制棋盘
				memcpy(nowflag, myflag, sizeof(flag));
				if (myplace(nowboard, myflag[cur_flag][i].x, myflag[cur_flag][i].y, k, cur_flag, nowflag))//放子
				{
					VAL = -Alpha_Beta(nowboard, depth - 1, -beta, -alpha, cur_other_flag, nowflag);//递归
					for (int a = myflag[cur_flag][i].x - 2; a <= myflag[cur_flag][i].x + 2; ++a)
					{
						if (a<0 || a>11) continue;
						for (int b = myflag[cur_flag][i].y - 2; b <= myflag[cur_flag][i].y + 2; ++b)
						{
							if (a<0 || a>11 || b<0 || b>11) continue;
							nowboard[a][b] = myboard[a][b];
						}
					}//保存棋盘
					memcpy(nowflag, myflag, sizeof(flag));
					//memcpy(nowboard, myboard, sizeof(board));

					if (VAL >= beta) return beta;

					if (VAL > alpha)
					{
						alpha = VAL;
					}
				}
			}
		}

	}
	return alpha;
}

//走子函数
struct Command MYAI()
{

	struct Command command = { 0, 0, 0 };

	char MYboard[12][12] = { 0 };
	Mplace MYflag[3][16];
	memcpy(MYboard, board, sizeof(board));
	memcpy(MYflag, flag, sizeof(flag));
	int nowvalue;
	int bestvalue = INT_MIN;
	int other_flag = 3 - me_flag;


	for (int i = 0; i < 16; ++i)
	{
		if (MYflag[me_flag][i].x == -1 || MYflag[me_flag][i].y == -1) continue;
		for (int k = 0; k < 8; k++)
		{
			if (MYboard[flag[me_flag][i].x + DIR[k][0]][flag[me_flag][i].y + DIR[k][1]] != EMPTY) continue;
			if (MYflag[me_flag][i].x + DIR[k][0]>11 || MYflag[me_flag][i].x + DIR[k][0]<0 || MYflag[me_flag][i].y + DIR[k][1]>11 || MYflag[me_flag][i].y + DIR[k][1] < 0) continue;
			myplace(MYboard, MYflag[me_flag][i].x, MYflag[me_flag][i].y, k, me_flag, MYflag);
			nowvalue = -Alpha_Beta(MYboard, 3, -INT_MAX, INT_MAX, other_flag, MYflag);
			for (int a = MYflag[me_flag][i].x - 2; a <= MYflag[me_flag][i].x + 2; ++a)
			{
				if (a<0 || a>11) continue;
				for (int b = MYflag[me_flag][i].y - 2; b <= MYflag[me_flag][i].y + 2; ++b)
				{
					if (a<0 || a>11 || b<0 || b>11) continue;
					MYboard[a][b] = board[a][b];
				}
			}
			memcpy(MYflag, flag, sizeof(flag));
			//保存棋盘
			//memcpy(MYboard, board, sizeof(board));
			if (bestvalue < nowvalue)
			{
				bestvalue = nowvalue;
				command.x = MYflag[me_flag][i].x;
				command.y = MYflag[me_flag][i].y;
				command.option = k;
			}
		}
	}

	return command;
}



/**
* 轮到你落子。
* 棋盘上0表示空白，1表示黑棋，2表示白棋
* me表示你所代表的棋子(1或2)
* 你需要返回一个结构体Command，在x属性和y属性填上你想要移动的棋子的位置，option填上你想要移动的方向。
*/
struct Command aiTurn(const char board[BOARD_SIZE][BOARD_SIZE], int me) {
	/*
	* TODO：在这里写下你的AI。
	* 这里有一个示例AI，它只会寻找第一个可下的位置进行落子。
	*/
	struct Command preferedPos = MYAI();

	return preferedPos;
}

/**
* 你的代码结束
*/


BOOL place(int x, int y, OPTION option, int cur_flag) {
	// 移动之前的位置没有我方棋子
	if (board[x][y] != cur_flag) {
		return FALSE;
	}

	int new_x = x + DIR[option][0];
	int new_y = y + DIR[option][1];
	// 移动之后的位置超出边界, 或者不是空地
	if (!isInBound(new_x, new_y) || board[new_x][new_y] != EMPTY) {
		return FALSE;
	}

	board[x][y] = EMPTY;
	board[new_x][new_y] = cur_flag;

	for (int i = 0; i < 16; ++i)
	{
		if (flag[cur_flag][i].x == x&&flag[cur_flag][i].y == y)
		{
			flag[cur_flag][i].x = new_x;
			flag[cur_flag][i].y = new_y;
			break;
		}
	}

	int cur_other_flag = 3 - cur_flag;

	// 挑
	int intervention_dir[4][2] = { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 1, -1 } };
	for (int i = 0; i < 4; i++) {
		int x1 = new_x + intervention_dir[i][0];
		int y1 = new_y + intervention_dir[i][1];
		int x2 = new_x - intervention_dir[i][0];
		int y2 = new_y - intervention_dir[i][1];
		if (isInBound(x1, y1) && isInBound(x2, y2) && board[x1][y1] == cur_other_flag && board[x2][y2] == cur_other_flag) {
			board[x1][y1] = cur_flag;
			board[x2][y2] = cur_flag;
			for (int i = 0; i < 16; ++i)
			{
				if (flag[cur_other_flag][i].x == x1&&flag[cur_other_flag][i].y == y1)
				{
					flag[cur_other_flag][i].x = -1;
					flag[cur_other_flag][i].y = -1;
					break;
				}
			}
			for (int i = 0; i < 16; ++i)
			{
				if (flag[cur_other_flag][i].x == x2&&flag[cur_other_flag][i].y == y2)
				{
					flag[cur_other_flag][i].x = -1;
					flag[cur_other_flag][i].y = -1;
					break;
				}
			}
			for (int i = 0; i < 16; ++i)
			{
				if (flag[cur_flag][i].x == -1 && flag[cur_flag][i].y == -1)
				{
					flag[cur_flag][i].x = x1;
					flag[cur_flag][i].y = y1;
					break;
				}
			}
			for (int i = 0; i < 16; ++i)
			{
				if (flag[cur_flag][i].x == -1 && flag[cur_flag][i].y == -1)
				{
					flag[cur_flag][i].x = x2;
					flag[cur_flag][i].y = y2;
					break;
				}
			}
		}
	}

	// 夹
	int custodian_dir[8][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } };
	for (int i = 0; i < 8; i++) {
		int x1 = new_x + custodian_dir[i][0];
		int y1 = new_y + custodian_dir[i][1];
		int x2 = new_x + custodian_dir[i][0] * 2;
		int y2 = new_y + custodian_dir[i][1] * 2;
		if (isInBound(x1, y1) && isInBound(x2, y2) && board[x2][y2] == cur_flag && board[x1][y1] == cur_other_flag) {
			board[x1][y1] = cur_flag;

			for (int i = 0; i < 16; ++i)
			{
				if (flag[cur_other_flag][i].x == x1&&flag[cur_other_flag][i].y == y1)
				{
					flag[cur_other_flag][i].x = -1;
					flag[cur_other_flag][i].y = -1;
					break;
				}
			}
			for (int i = 0; i < 16; ++i)
			{
				if (flag[cur_flag][i].x == -1 && flag[cur_flag][i].y == -1)
				{
					flag[cur_flag][i].x = x1;
					flag[cur_flag][i].y = y1;
					break;
				}
			}
		}
	}
	return TRUE;
}



void start(int oflag) {//初始化记录黑白子，并对其顺序进行调换，优先搜索中间子，提高速度（有点类似启发式搜索）
	memset(board, 0, sizeof(board));
	memset(flag, -1, sizeof(flag));


	for (int i = 0; i < 3; i++) {
		board[2][2 + i] = WHITE;
		flag[WHITE][i + 3].x = 2;
		flag[WHITE][i + 3].y = 2 + i;
		board[6][6 + i] = WHITE;
		flag[WHITE][i].x = 6;
		flag[WHITE][i].y = 6 + i;
		board[5][3 + i] = BLACK;
		flag[BLACK][i].x = 5;
		flag[BLACK][i].y = 3 + i;
		board[9][7 + i] = BLACK;
		flag[BLACK][i + 3].x = 9;
		flag[BLACK][i + 3].y = 7 + i;
	}

	for (int i = 0; i < 2; i++) {
		board[8 + i][2] = WHITE;
		flag[WHITE][6 + i].x = 8 + i;
		flag[WHITE][6 + i].y = 2;
		board[2 + i][9] = BLACK;
		flag[BLACK][6 + i].x = 2 + i;
		flag[BLACK][6 + i].y = 9;
	}
	initAI(oflag);
}
//加EMPTY
void turn() {
	// AI


	if (board[6][6] == other_flag &&board[6][7] == other_flag&&board[6][8] == other_flag&&board[9][8] == me_flag  && board[9][9] == me_flag&&board[9][7] == me_flag&&board[8][6] == EMPTY)
	{
		place(9, 7, 4, me_flag);
		printf("9 7 4\n");
		fflush(stdout);
	}///////////////////////手动place
	/*	else if (board[6][6] == other_flag &&board[6][7] == other_flag&&board[8][8] == me_flag  && board[5][5] == me_flag&&board[7][7] == EMPTY)
	{
	place(8, 8, 4, me_flag);
	printf("8 8 4\n");
	fflush(stdout);
	}
	else if (board[6][7] == other_flag&&board[6][8] == other_flag&&board[7][7] == me_flag  && board[6][6] == me_flag&&board[5][7] == EMPTY)
	{
	place(6, 6, 5, me_flag);
	printf("6 6 5\n");
	fflush(stdout);
	}
	else if (board[6][7] == me_flag&&board[6][8] == other_flag&&board[7][7] == me_flag  && board[5][7] == me_flag&&board[3][9] == me_flag&&board[4][8] == EMPTY)
	{
	place(3, 9, 6, me_flag);
	printf("3 9 6\n");
	fflush(stdout);
	}
	else if (board[6][7] == me_flag&&board[6][8] == other_flag&&board[7][7] == me_flag  && board[5][7] == me_flag&&board[4][8] == me_flag&&board[5][9] == EMPTY)
	{
	place(4, 8, 7, me_flag);
	printf("4 8 7\n");
	fflush(stdout);
	}
	else if ((board[8][6] == other_flag || board[8][7] == other_flag || board[8][8] == other_flag) && board[6][6] == me_flag&&board[5][7] == EMPTY)
	{
	place(6, 6, 5, me_flag);
	printf("6 6 5\n");
	fflush(stdout);
	}
	else if (board[2][3] == me_flag && board[5][3] == other_flag && board[5][4] == other_flag && board[5][5] == other_flag&&board[6][6] == me_flag&&board[3][3] == EMPTY && board[8][6] != other_flag && board[8][7] != other_flag && board[8][8] != other_flag)
	{
	place(2, 3, 1, me_flag);
	printf("2 3 1\n");
	fflush(stdout);
	}
	else if (board[3][3] == me_flag && board[5][3] == other_flag && board[5][4] == other_flag && board[5][5] == other_flag&&board[6][6] == me_flag&&board[4][4] == EMPTY)
	{
	place(3, 3, 7, me_flag);
	printf("3 3 7\n");
	fflush(stdout);
	}
	else if (board[4][4] == me_flag && board[5][3] == other_flag && board[5][4] == other_flag && board[5][5] == me_flag&&board[6][4] == EMPTY)
	{
	place(5, 5, 6, me_flag);
	printf("5 5 6\n");
	fflush(stdout);
	}
	else if (board[5][4] == me_flag && board[5][3] == other_flag && board[4][4] == me_flag && board[6][5] == me_flag&&board[8][2] == me_flag&&board[7][3] == EMPTY)
	{
	place(8, 2, 5, me_flag);
	printf("8 2 5\n");
	fflush(stdout);
	}
	else if (board[5][4] == me_flag && board[5][3] == other_flag && board[4][4] == me_flag && board[6][5] == me_flag&&board[7][3] == me_flag&&board[6][2] == EMPTY)
	{
	place(7, 3, 4, me_flag);
	printf("7 3 4\n");
	fflush(stdout);
	}
	*/
	else

	{
		struct Command command = aiTurn((const char(*)[BOARD_SIZE])board, me_flag);
		place(command.x, command.y, command.option, me_flag);
		printf("%d %d %d\n", command.x, command.y, command.option);
		fflush(stdout);
	}
}

void end(int x) {

}

void loop() {
	//  freopen("../input", "r", stdin);
	while (TRUE)
	{
		memset(buffer, 0, sizeof(buffer));
		gets(buffer);

		if (strstr(buffer, START))
		{
			char tmp[MAX_BYTE] = { 0 };
			sscanf(buffer, "%s %d", tmp, &me_flag);
			other_flag = 3 - me_flag;
			start(me_flag);
			printf("OK\n");
			fflush(stdout);

		}
		else if (strstr(buffer, PLACE))
		{
			char tmp[MAX_BYTE] = { 0 };
			int x, y;
			OPTION option;

			sscanf(buffer, "%s %d %d %d", tmp, &x, &y, &option);
			place(x, y, option, other_flag);
		}
		else if (strstr(buffer, TURN))
		{
			turn();
		}
		else if (strstr(buffer, END))
		{
			char tmp[MAX_BYTE] = { 0 };
			int x;
			sscanf(buffer, "%s %d", tmp, &x);
			end(x);
		}
		//printBoard();
	}
}

int main(int argc, char *argv[]) {
	loop();
	return 0;
}