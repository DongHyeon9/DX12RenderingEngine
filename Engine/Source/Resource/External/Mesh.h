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

	void SetMeshData(const std::vector<MeshData>& Meshes);
	void AddMeshData(const MeshData& Mesh);
	FORCEINLINE const std::vector<MeshData>& GetMesh()const { return meshes; }
protected:

private:
	void ProcessNode(aiNode* Node, const aiScene* Scene, Matrix Transform);
	MeshData ProcessMesh(aiMesh* Mesh, const aiScene* Scene);

};

