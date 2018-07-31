#pragma once
#include <Siv3D.hpp>
#include "Scene.h"

namespace Management
{
	void Init(Scene *firstScene);
	void SceneChange(Scene *newScene);
	void Management();
};
