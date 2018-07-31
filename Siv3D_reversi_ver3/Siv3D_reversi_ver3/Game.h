#pragma once
#include <Siv3D.hpp>

#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>

#include "ReversiLog.h"

#include "Scene.h"

#define COLOR_BLACK -1
#define COLOR_WHITE  1

typedef enum {
	e_next,
	e_inputWait,
	e_inputCheck,

	e_none,
} eStep;

static int check_seet[8][8]{
	{  30, -12,  0, -1, -1,  0, -12,  30 },
	{ -12, -25, -3, -3, -3, -3, -25, -12 },
	{   0,  -3,  0, -1, -1,  0,  -3,   0 },
	{  -1,  -3, -1, -1, -1, -1,  -3,  -1 },
	{  -1,  -3, -1, -1, -1, -1,  -3,  -1 },
	{   0,  -3,  0, -1, -1,  0,  -3,   0 },
	{ -12, -25, -3, -3, -3, -3, -25, -12 },
	{  30, -12,  0, -1, -1,  0, -12,  30 },
};

/*
{   5, -5, 3, 3, 3, 3, -5, 5 },
{ 20,  -5, 15,  3,  3, 15,  -5,  20 },
{ -10, -20, -5, -5, -5, -5, -20, -10 },
{ 60, -10, 20,  5,  5, 20, -10,  60 },
*/

class Game : public Scene
{
private:
	eStep step;

	int turn;
	int turn_count;
	int depth = 2;
	int board[8][8];

	//ある手番の人が置ける場所を保存する変数
	std::vector<std::pair<int, std::vector<std::pair<int, int>>>>moves;

	void Next();
	bool PlayerInput(int, int&, int&);

	ReversiLog log_data;

	Font font_text;

	void Wing();
	void Stone();
public:
	Game();
	~Game();

	void Enter();
	void Exit();
	void Update();
	void Draw();
};

//８方向を調べる
std::vector< std::pair<int, int> > get_directions(int board[][8], int x, int y, int color);

//置ける場所を調べる
std::vector< std::pair<int, std::vector< std::pair<int, int> > > > get_moves(int board[][8], int color);

//
void make_move(int board[][8], int x, int y, int color, std::vector< std::pair<int, int> > directions);

//
void undo_move(int board[][8], int x, int y, int color, std::vector< std::pair<int, int> > directions);

//スコアを出す
int score(int board[][8], int color);

//
int negamax_aux(int board[][8], int color, int depth, int alpha, int beta);

//
int negamax(int board[][8], int color, int depth);

int input_ai_checkseet(int board[][8], int color);
int input_ai_count_check(int board[][8], int color);
int input_ai_count_check2(int board[][8], int color);

//盤面に指定した色の駒が何個あるか返す
int color_count(int board[][8], int color);