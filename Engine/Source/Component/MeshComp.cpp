#include "MeshComp.h"

#include "Manager\RenderManager.h"
#include "Engine\Render\DeviceObject.h"
#include "Engine\Render\CommandObject.h"
#include "Engine\Render\PipelineStateObject.h"
#include "Engine\Render\PipelineStateObject\RootSignatureObject.h"

#include "Resource\External\Mesh.h"
#include "Resource\Internal\Material.h"

bool MeshComp::Init(std::shared_ptr<Mesh> Mesh)
{
	LOG("메쉬 렌더러 컴포넌트 초기화 시작");
	mesh = Mesh;
	auto meshesData = mesh->GetMesh();
	size_t size = meshesData.size();

	CHECK(InitBufferSize(size), "버퍼사이즈 설정 실패", false);
	CHECK(CreateMaterial(size), "머티리얼 생성 실패", false);
	CHECK(CreateVertexBuffer(meshesData), "버텍스 버퍼 생성 실패", false);
	CHECK(CreateIndexBuffer(meshesData), "인덱스 버퍼 생성 실패", false);

	LOG("메쉬 렌더러 컴포넌트 초기화 성공");
	return true;
}

bool MeshComp::Init(const MeshData& MeshData)
{
	LOG("메쉬 렌더러 컴포넌트 초기화 시작");

	mesh = std::make_shared<Mesh>();
	mesh->SetMeshData({ MeshData });

	LOG("메쉬 렌더러 컴포넌트 초기화 성공");
	return true;
}

void MeshComp::SetMesh(std::shared_ptr<Mesh> Mesh)
{
	mesh = Mesh;
	auto meshesData = mesh->GetMesh();
	size_t size = meshesData.size();

	CHECK(InitBufferSize(size), "버퍼사이즈 설정 실패", );
	CHECK(CreateMaterial(size), "머티리얼 생성 실패", );
	CHECK(CreateVertexBuffer(meshesData), "버텍스 버퍼 생성 실패", );
	CHECK(CreateIndexBuffer(meshesData), "인덱스 버퍼 생성 실패", );
}

void MeshComp::Render()
{
	__super::Render();

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList = CMD_OBJ->GetCommandList();
	std::shared_ptr<RootSignatureObject> rootSignature = PSO->GetRootSignature();
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (size_t i = 0; i < materials.size(); ++i)
	{
		cmdList->IASetVertexBuffers(0, 1, &vertexBufferView[i]);
		cmdList->IASetIndexBuffer(&indexBufferView[i]);
		materials[i]->Render();
		rootSignature->CommitDescriptor();
		cmdList->DrawIndexedInstanced(indexCount[i], 1, 0, 0, 0);
	}

	//if (GEngine->GetRenderSetting().bIsDrawNormal)
	//{
	//	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//	for (size_t i = 0; i < materials.size(); ++i)
	//	{
	//		cmdList->IASetVertexBuffers(0, 1, &vertexBufferView[i]);
	//		CommandQueue::GetInstance()->SetPipelineState(RENDERING::NORMAL);
	//		cmdList->DrawInstanced(vertexCount[i], 1, 0, 0);
	//	}
	//}
}

void MeshComp::SetMaterial(uint8 Index, std::shared_ptr<Material> Material)
{
	if (materials.size() - 1 < Index)
	{
		LOG("머티리얼 인덱스 범위 초과");
		return;
	}
	materials[Index] = Material;
}

bool MeshComp::InitBufferSize(size_t Size)
{
	LOG("버퍼 사이즈 설정 시작");

	vertexBuffer.resize(Size);
	vertexBufferView.resize(Size);
	vertexCount.resize(Size);
	indexBuffer.resize(Size);
	indexBufferView.resize(Size);
	indexCount.resize(Size);
	materials.resize(Size);

	LOG("버퍼 사이즈 설정 성공");
	return true;
}

bool MeshComp::CreateMaterial(size_t Size)
{
	for (size_t i = 0; i < Size; ++i)
	{
		materials[i] = std::make_shared<Material>();
	}

	return true;
}

bool MeshComp::CreateVertexBuffer(const std::vector<MeshData>& MeshesData)
{
	LOG("버텍스 버퍼 생성 시작");

	for (size_t i = 0; i < MeshesData.size(); ++i)
	{
		vertexCount[i] = static_cast<uint32>(MeshesData[i].vertices.size());
		uint32 bufferSize = vertexCount[i] * sizeof(Vertex);

		D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_UPLOAD };
		D3D12_RESOURCE_DESC desc{ CD3DX12_RESOURCE_DESC::Buffer(bufferSize) };

		HRESULT hr{};
		hr = DEVICE_OBJ->GetDevice()->CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertexBuffer[i]));
		CHECK(SUCCEEDED(hr), "버텍스 버퍼 커밋 실패", false);
		LOG("버텍스 버퍼 커밋");

		void* vertexDataBuffer{};
		CD3DX12_RANGE readRange{ 0,0 };
		vertexBuffer[i]->Map(0, &readRange, &vertexDataBuffer);
		memcpy(vertexDataBuffer, &MeshesData[i].vertices[0], bufferSize);
		vertexBuffer[i]->Unmap(0, nullptr);

		vertexBufferView[i].BufferLocation = vertexBuffer[i]->GetGPUVirtualAddress();
		vertexBufferView[i].StrideInBytes = sizeof(Vertex);
		vertexBufferView[i].SizeInBytes = bufferSize;
	}

	LOG("버텍스 버퍼 생성 성공");
    return true;
}

bool MeshComp::CreateIndexBuffer(const std::vector<MeshData>& MeshesData)
{
	LOG("인덱스 버퍼 생성 시작");

	for (size_t i = 0; i < MeshesData.size(); ++i)
	{
		indexCount[i] = static_cast<uint32>(MeshesData[i].indices.size());
		uint32 bufferSize = indexCount[i] * sizeof(uint32);

		D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_UPLOAD };
		D3D12_RESOURCE_DESC desc{ CD3DX12_RESOURCE_DESC::Buffer(bufferSize) };

		HRESULT hr{};
		hr = DEVICE_OBJ->GetDevice()->CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuffer[i]));
		CHECK(SUCCEEDED(hr), "인덱스 버퍼 커밋 실패", false);
		LOG("인덱스 버퍼 커밋");

		void* indexDataBuffer{};
		CD3DX12_RANGE readRange{ 0,0 };
		indexBuffer[i]->Map(0, &readRange, &indexDataBuffer);
		memcpy(indexDataBuffer, &MeshesData[i].indices[0], bufferSize);
		indexBuffer[i]->Unmap(0, nullptr);

		indexBufferView[i].BufferLocation = indexBuffer[i]->GetGPUVirtualAddress();
		indexBufferView[i].Format = DXGI_FORMAT_R32_UINT;
		indexBufferView[i].SizeInBytes = bufferSize;
	}

	LOG("인덱스 버퍼 생성 성공");
    return true;
}
