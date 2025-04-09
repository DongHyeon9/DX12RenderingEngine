#pragma once
#include "Header\EngineCore.h"

class Resource : public std::enable_shared_from_this<Resource>
{
public:

protected:
	FString fileName{};
	FString path{};

private:

public:
	Resource(FString Path) :path(Path) {}

	FORCEINLINE virtual bool Load(FString FileName) { fileName = std::move(FileName); return true; }
	FORCEINLINE std::shared_ptr<Resource> GetResource() { return shared_from_this(); };
	FORCEINLINE FString GetFileName()const { return fileName; }
	FORCEINLINE FString GetPath()const { return path; }

protected:

private:

};