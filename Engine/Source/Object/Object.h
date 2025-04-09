#pragma once
#include "Header\EngineCore.h"

class Object : public std::enable_shared_from_this<Object>
{
public:

protected:

private:
	FString objectName{};
	bool bDestroyFlag{};

public:
	Object(FString ObjectName) :objectName(std::move(ObjectName)) {}
	void Destroy();
	FORCEINLINE bool IsDestroy()const { return bDestroyFlag; }
	virtual void BeginDestroy() {}

protected:

private:

};

