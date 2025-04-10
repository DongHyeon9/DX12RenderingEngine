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
	Component(FString ObjName) :Object(std::move(ObjName)) {}
	void SetOwner(std::shared_ptr<Actor> Owner);
	virtual void Update(float DeltaTime) {}
	virtual void LateUpdate(float DeltaTime) {}
	virtual void BeginPlay() {}

protected:

private:

};

