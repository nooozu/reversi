#include "Result.h"
#include "Management.h"
#include "Title.h"

Result::Result(int b, int w)
{
	blackCount = b;
	whiteCount = w;

}

Result::~Result()
{
}

void Result::Enter()
{
	Graphics::SetBackground(Palette::Gray);

	font_title = Font(30);
	font_text = Font(24);
	font_back = Font(18);
	back = Rect(320 - 50, 330 - 20, 100, 40);
}

void Result::Exit()
{

}

void Result::Update()
{
	if (Input::MouseL.pressed) {
		if (back.mouseOver) {
			Management::SceneChange(new Title());
		}
	}
}

void Result::Draw()
{
	font_title(L"Result").drawCenter(320, 140, Palette::Black);

	font_text(L"black : ", blackCount, L"  vs  white : ", whiteCount).drawCenter(320, 220, Palette::Blue);

	back.draw(Palette::Black);
	if (back.mouseOver) {
		back.drawFrame(3.0, 0.0, Palette::White);
	}
	font_back(L"Back").draw(back.pos);
}