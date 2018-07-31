#pragma once
class Scene
{
public:
	Scene();
	~Scene();

	virtual void Enter();
	virtual void Exit();
	virtual void Update();
	virtual void Draw();
};