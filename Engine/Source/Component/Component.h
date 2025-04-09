#pragma once
#include "Object\Object.h"

class Actor;

class Component : public Object
{
public:

protected:
	std::weak_ptr<Actor> owner{};

private:

public:
	void SetOwner(std::shared_ptr<Actor> Owner);

protected:

private:

};

