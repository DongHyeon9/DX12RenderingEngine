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
	LOG("콘트탄트 버퍼 초기화");
	cbvRegister = Register;

	//256의 배수로 만들어줌
	elementSize = EngineUtil::Shader::CalulateConstantBufferSize(Size);
	elementCount = Count;

	CHECK(CreateBuffer(), "버퍼 생성 실패", false);
	CHECK(CreateView(), "뷰 생성 실패", false);

	LOG("콘트탄트 버퍼 초기화 성공");
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
	LOG("버퍼 생성 시작");
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

	CHECK(SUCCEEDED(hr), "리소스 커밋 실패", false);
	LOG("리소스 커밋");

	hr = cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedBuffer));
	CHECK(SUCCEEDED(hr), "버퍼 맵핑 실패", false);
	LOG("버퍼 맵핑");

	LOG("버퍼 생성 완료");
	return true;
}

bool ConstantBuffer::CreateView()
{
	LOG("뷰 생성 시작");

	Microsoft::WRL::ComPtr<ID3D12Device> device = DEVICE_OBJ->GetDevice();

	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.NumDescriptors = elementCount;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	HRESULT hr{};
	hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&cbvHeap));
	CHECK(SUCCEEDED(hr), "디스크립터 힙 생성 실패", false);
	LOG("디스크립터 힙 생성");

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

	LOG("뷰 생성 시작 완료");
	return true;
}
