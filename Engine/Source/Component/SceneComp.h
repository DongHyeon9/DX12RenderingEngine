#pragma once
#include "Component\Component.h"

class SceneComp : public Component
{
public:

protected:
	std::weak_ptr<SceneComp> parent{};
	std::vector<std::shared_ptr<SceneComp>> children{};

private:
	Transform relativeTransform{};
	Matrix world{};
	Matrix local{};

	Vector3 forward{};
	Vector3 right{};
	Vector3 up{};

public:

protected:

private:

};

