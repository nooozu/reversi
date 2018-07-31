#pragma once
#include "Scene.h"
#include <Siv3D.hpp>

#define WIDTH 640
#define HEIGHT 480

class Title : public Scene
{
	Font font_title, font_button;
	Rect play, replay;

public:
	Title();
	~Title();

	void Enter();
	void Exit();
	void Update();
	void Draw();
};

