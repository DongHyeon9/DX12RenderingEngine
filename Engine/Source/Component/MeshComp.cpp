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
	LOG("�޽� ������ ������Ʈ �ʱ�ȭ ����");
	mesh = Mesh;
	auto meshesData = mesh->GetMesh();
	size_t size = meshesData.size();

	CHECK(InitBufferSize(size), "���ۻ����� ���� ����", false);
	CHECK(CreateMaterial(size), "��Ƽ���� ���� ����", false);
	CHECK(CreateVertexBuffer(meshesData), "���ؽ� ���� ���� ����", false);
	CHECK(CreateIndexBuffer(meshesData), "�ε��� ���� ���� ����", false);

	LOG("�޽� ������ ������Ʈ �ʱ�ȭ ����");
	return true;
}

bool MeshComp::Init(const MeshData& MeshData)
{
	LOG("�޽� ������ ������Ʈ �ʱ�ȭ ����");

	mesh = std::make_shared<Mesh>();
	mesh->SetMeshData({ MeshData });

	LOG("�޽� ������ ������Ʈ �ʱ�ȭ ����");
	return true;
}

void MeshComp::SetMesh(std::shared_ptr<Mesh> Mesh)
{
	mesh = Mesh;
	auto meshesData = mesh->GetMesh();
	size_t size = meshesData.size();

	CHECK(InitBufferSize(size), "���ۻ����� ���� ����", );
	CHECK(CreateMaterial(size), "��Ƽ���� ���� ����", );
	CHECK(CreateVertexBuffer(meshesData), "���ؽ� ���� ���� ����", );
	CHECK(CreateIndexBuffer(meshesData), "�ε��� ���� ���� ����", );
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
		LOG("��Ƽ���� �ε��� ���� �ʰ�");
		return;
	}
	materials[Index] = Material;
}

bool MeshComp::InitBufferSize(size_t Size)
{
	LOG("���� ������ ���� ����");

	vertexBuffer.resize(Size);
	vertexBufferView.resize(Size);
	vertexCount.resize(Size);
	indexBuffer.resize(Size);
	indexBufferView.resize(Size);
	indexCount.resize(Size);
	materials.resize(Size);

	LOG("���� ������ ���� ����");
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
	LOG("���ؽ� ���� ���� ����");

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
		CHECK(SUCCEEDED(hr), "���ؽ� ���� Ŀ�� ����", false);
		LOG("���ؽ� ���� Ŀ��");

		void* vertexDataBuffer{};
		CD3DX12_RANGE readRange{ 0,0 };
		vertexBuffer[i]->Map(0, &readRange, &vertexDataBuffer);
		memcpy(vertexDataBuffer, &MeshesData[i].vertices[0], bufferSize);
		vertexBuffer[i]->Unmap(0, nullptr);

		vertexBufferView[i].BufferLocation = vertexBuffer[i]->GetGPUVirtualAddress();
		vertexBufferView[i].StrideInBytes = sizeof(Vertex);
		vertexBufferView[i].SizeInBytes = bufferSize;
	}

	LOG("���ؽ� ���� ���� ����");
    return true;
}

bool MeshComp::CreateIndexBuffer(const std::vector<MeshData>& MeshesData)
{
	LOG("�ε��� ���� ���� ����");

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
		CHECK(SUCCEEDED(hr), "�ε��� ���� Ŀ�� ����", false);
		LOG("�ε��� ���� Ŀ��");

		void* indexDataBuffer{};
		CD3DX12_RANGE readRange{ 0,0 };
		indexBuffer[i]->Map(0, &readRange, &indexDataBuffer);
		memcpy(indexDataBuffer, &MeshesData[i].indices[0], bufferSize);
		indexBuffer[i]->Unmap(0, nullptr);

		indexBufferView[i].BufferLocation = indexBuffer[i]->GetGPUVirtualAddress();
		indexBufferView[i].Format = DXGI_FORMAT_R32_UINT;
		indexBufferView[i].SizeInBytes = bufferSize;
	}

	LOG("�ε��� ���� ���� ����");
    return true;
}
