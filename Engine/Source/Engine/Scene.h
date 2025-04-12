#pragma once
#include "Header\EngineCore.h"

class Actor;

class Scene
{
public:

protected:

private:
	std::vector<std::shared_ptr<Actor>> children{};
	std::queue<std::shared_ptr<Actor>> addChildren{};

public:
	bool Init();
	void OnEnterScene();
	void OnExitScene();

	void Update(float DeltaTime);
	void LateUpdate(float DeltaTime);
	void Render();

	void AddActor(std::shared_ptr<Actor> Actor);
protected:

private:

};

