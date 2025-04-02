#pragma once
#include "Header\EngineCore.h"

template<class _ResourceType>
class Resource
{
public:

protected:
	FString fileName{};
	FString path{};

private:

public:
	Resource(FString Path) :path(Path) {}

	virtual bool Load(FString FileName);
	FORCEINLINE virtual _ResourceType GetResource()const abstract;

protected:

private:

};

#include "Resource.inl"