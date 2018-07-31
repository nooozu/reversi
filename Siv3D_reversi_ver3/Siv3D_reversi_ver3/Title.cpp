#include "Title.h"
#include "Game.h"
#include "Replay.h"
#include "Management.h"

Title::Title()
{
}

Title::~Title()
{
}

void Title::Enter()
{
	Graphics::SetBackground(Palette::White);

	font_title = Font(35);
	font_button = Font(18);

	play = Rect(320 - 50 - 100, 340 - 20, 100, 40);
	replay = Rect(320 + 50, 340 - 20, 100, 40);

}

void Title::Exit()
{

}

void Title::Update()
{
	if (Input::MouseL.pressed) {
		if (play.mouseOver) {
			Management::SceneChange(new Game());
		}
		if (replay.mouseOver) {
			Management::SceneChange(new Replay());
		}
	}
}

void Title::Draw()
{
	font_title(L"Reversi").drawCenter(320, 120, Palette::Black);

	play.draw(Palette::Lightblue);
	if (play.mouseOver) {
		play.drawFrame(3.0, 0.0, Palette::Blue);
	}

	replay.draw(Palette::Lightgreen);
	if (replay.mouseOver) {
		replay.drawFrame(3.0, 0.0, Palette::Green);
	}

	font_button(L"Play").draw(play.pos, Palette::Blue);
	font_button(L"Replay").draw(replay.pos, Palette::Green);
}