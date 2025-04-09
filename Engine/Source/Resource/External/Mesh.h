#pragma once
#include "Resource.h"

class Mesh : public Resource
{
public:

protected:

private:
	std::vector<MeshData> meshes{};

public:
	Mesh() :Resource(EngineUtil::Path::GetMeshDir()) {}
	bool Load(FString FileName)override;

protected:

private:

};

