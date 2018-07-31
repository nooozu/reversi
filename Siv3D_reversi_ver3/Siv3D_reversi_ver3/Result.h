#pragma once
#include "Scene.h"
#include <Siv3D.hpp>

class Result : public Scene
{
private:
	int blackCount, whiteCount;

	Font font_title, font_text, font_back;
	Rect back;
	
public:
	Result(int, int);
	~Result();

	void Enter();
	void Exit();
	void Update();
	void Draw();
};

