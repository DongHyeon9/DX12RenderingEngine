#pragma once
#include "Header\EngineCore.h"

class Object : public std::enable_shared_from_this<Object>
{
public:

protected:

private:
	FString objectName{};

public:
	Object(FString ObjectName) :objectName(std::move(ObjectName)) {}

protected:

private:

};

