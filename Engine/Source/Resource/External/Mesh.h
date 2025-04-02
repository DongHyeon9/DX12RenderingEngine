#pragma once
#include "Resource.h"

class Mesh : public Resource<Mesh>
{
public:

protected:

private:
	std::vector<MeshData> meshes{};

public:
	Mesh() :Resource<Mesh>(EngineUtil::Path::GetMeshDir()) {}
	bool Load(FString FileName)override;

protected:

private:

};

