#include "Management.h"

Scene *nowScene;
Scene *nextScene;

void Management::Init(Scene *firstScene)
{
	nowScene = firstScene;
	nowScene->Enter();

	nextScene = NULL;
}

void Management::Management()
{
	while (System::Update()) {
		nowScene->Update();
		nowScene->Draw();
		if (nextScene != NULL) {
			nowScene->Exit();
			delete nowScene;
			nowScene = nextScene;
			nowScene->Enter();
			nextScene = NULL;
		}
	}
}

void Management::SceneChange(Scene *newScene)
{
	nextScene = newScene;
}