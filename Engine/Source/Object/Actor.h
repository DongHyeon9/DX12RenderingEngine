#pragma once
#include "Object\Object.h"

class Scene;
class Component;
class SceneComp;

class Actor : public Object
{
public:

protected:

private:
	bool bIsDestroyFlag{};
	std::weak_ptr<Scene> currentScene{};
	std::weak_ptr<Actor> parent{};
	std::vector<std::shared_ptr<Actor>> children{};

	std::shared_ptr<SceneComp> rootComponent{};
	std::vector<std::shared_ptr<Component>> actorComponents{};
	std::vector<std::shared_ptr<SceneComp>> sceneComponents{};

public:

protected:

private:

};

