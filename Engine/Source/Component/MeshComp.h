#pragma once
#include "Component\SceneComp.h"

class Mesh;
class Material;

class MeshComp : public SceneComp
{
public:

protected:

private:
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> vertexBuffer{};
	std::vector<D3D12_VERTEX_BUFFER_VIEW> vertexBufferView{};
	std::vector<uint32> vertexCount{};

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> indexBuffer{};
	std::vector<D3D12_INDEX_BUFFER_VIEW> indexBufferView{};
	std::vector<uint32> indexCount{};

	std::vector<std::shared_ptr<Material>> materials{};

	std::shared_ptr<Mesh> mesh{};

public:
	MeshComp(FString ObjName) :SceneComp(ObjName) {}
	bool Init(std::shared_ptr<Mesh> Mesh);
	bool Init(const MeshData& MeshData);
	void SetMesh(std::shared_ptr<Mesh> Mesh);
	void Render()override;
	void SetMaterial(uint8 Index, std::shared_ptr<Material> Material);

protected:

private:
	bool InitBufferSize(size_t Size);
	bool CreateMaterial(size_t Size);
	bool CreateVertexBuffer(const std::vector<MeshData>& MeshesData);
	bool CreateIndexBuffer(const std::vector<MeshData>& MeshesData);

};

