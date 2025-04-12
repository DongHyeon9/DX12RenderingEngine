#include "ConstantBuffer.h"

#include "Manager\RenderManager.h"
#include "Engine\Render\DeviceObject.h"
#include "Engine\Render\CommandObject.h"

ConstantBuffer::~ConstantBuffer()
{
	if (cbvBuffer)
	{
		if (cbvBuffer != nullptr)
			cbvBuffer->Unmap(0, nullptr);

		cbvBuffer = nullptr;
	}
}

bool ConstantBuffer::Init(E_CBV_REGISTER Register, uint32 Size, uint32 Count)
{
	LOG("��ƮźƮ ���� �ʱ�ȭ");
	cbvRegister = Register;

	//256�� ����� �������
	elementSize = EngineUtil::Shader::CalulateConstantBufferSize(Size);
	elementCount = Count;

	CHECK(CreateBuffer(), "���� ���� ����", false);
	CHECK(CreateView(), "�� ���� ����", false);

	LOG("��ƮźƮ ���� �ʱ�ȭ ����");
	return true;
}

void ConstantBuffer::Clear()
{
	currentIndex = 0;
}

D3D12_CPU_DESCRIPTOR_HANDLE ConstantBuffer::PushData(void* Buffer, uint32 Size)
{
	assert(currentIndex < elementCount);
	assert(elementSize == (EngineUtil::Shader::CalulateConstantBufferSize(Size)));

	memcpy(&mappedBuffer[currentIndex * elementSize], Buffer, Size);

	return GetCPUHandle(currentIndex++);
}

void ConstantBuffer::PushGlobalData(void* Buffer, uint32 Size)
{
	assert(elementSize == (EngineUtil::Shader::CalulateConstantBufferSize(Size)));
	memcpy(&mappedBuffer[0], Buffer, Size);
	CMD_OBJ->GetCommandList()->SetGraphicsRootConstantBufferView(static_cast<uint32>(cbvRegister), GetGPUVirtualAddress(0));
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGPUVirtualAddress(uint32 Index)
{
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += Index * elementSize;
	return objCBAddress;
}

D3D12_CPU_DESCRIPTOR_HANDLE ConstantBuffer::GetCPUHandle(uint32 Index)
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE{ cpuHandleBegin, static_cast<INT>(Index * handleIncreamentSize) };
}

bool ConstantBuffer::CreateBuffer()
{
	LOG("���� ���� ����");
	uint32 bufferSize = elementSize * elementCount;
	D3D12_HEAP_PROPERTIES heapProperty{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) };
	D3D12_RESOURCE_DESC desc{ CD3DX12_RESOURCE_DESC::Buffer(bufferSize) };

	HRESULT hr{};
	hr = DEVICE_OBJ->GetDevice()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&cbvBuffer));

	CHECK(SUCCEEDED(hr), "���ҽ� Ŀ�� ����", false);
	LOG("���ҽ� Ŀ��");

	hr = cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedBuffer));
	CHECK(SUCCEEDED(hr), "���� ���� ����", false);
	LOG("���� ����");

	LOG("���� ���� �Ϸ�");
	return true;
}

bool ConstantBuffer::CreateView()
{
	LOG("�� ���� ����");

	Microsoft::WRL::ComPtr<ID3D12Device> device = DEVICE_OBJ->GetDevice();

	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.NumDescriptors = elementCount;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	HRESULT hr{};
	hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&cbvHeap));
	CHECK(SUCCEEDED(hr), "��ũ���� �� ���� ����", false);
	LOG("��ũ���� �� ����");

	cpuHandleBegin = cbvHeap->GetCPUDescriptorHandleForHeapStart();
	handleIncreamentSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (uint32 i = 0; i < elementCount; ++i)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GetCPUHandle(i);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
		cbvDesc.BufferLocation = cbvBuffer->GetGPUVirtualAddress() + static_cast<uint64>(elementSize) * i;
		cbvDesc.SizeInBytes = elementSize;

		device->CreateConstantBufferView(&cbvDesc, handle);
	}

	LOG("�� ���� ���� �Ϸ�");
	return true;
}
