#pragma once
#include "ReversiLog.h"
#include "Game.h"

class Replay : public Scene
{
private:
	ReversiLog log;
	int replay_count, frame;


	eStep step;
	int turn;
	int turn_count;
	int depth = 1;
	int board[8][8];
	//�����Ԃ̐l���u����ꏊ��ۑ�����ϐ�
	std::vector<std::pair<int, std::vector<std::pair<int, int>>>>moves;

	void Next();
	//bool PlayerInput(int, int&, int&);

	ReversiLog log_data;

	Font font_text;
public:
	Replay();
	~Replay();

	void Enter();
	void Exit();
	void Update();
	void Draw();
};

