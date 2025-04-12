#include "DescriptorTable.h"

#include "Manager\RenderManager.h"
#include "Engine\Render\DeviceObject.h"
#include "Engine\Render\CommandObject.h"

bool DescriptorTable::Init(uint32 Count)
{
	LOG("디스크립터 힙 초기화 시작");

	Microsoft::WRL::ComPtr<ID3D12Device> device = DEVICE_OBJ->GetDevice();

	groupCount = Count;

	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.NumDescriptors = Count * (LITERAL::REGISTER_COUNT - LITERAL::GLOBAL_CBUFFER_COUNT);	//b0 카메라, b1라이트
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	HRESULT hr{};
	hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descHeap));
	CHECK(SUCCEEDED(hr), "디스크립터 힙 생성 실패", false);
	LOG("디스크립터 힙 생성");

	handleSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	groupSize = handleSize * (LITERAL::REGISTER_COUNT - LITERAL::GLOBAL_CBUFFER_COUNT);	//b0 카메라, b1라이트

	LOG("디스크립터 힙 초기화 성공");
	return true;
}

void DescriptorTable::Clear()
{
	currentGroupIndex = 0;
}

void DescriptorTable::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, E_CBV_REGISTER Register)
{
	D3D12_CPU_DESCRIPTOR_HANDLE dstHandle = GetCPUHandle(Register);

	uint32 dstRange = 1;
	uint32 srcRange = 1;
	DEVICE_OBJ->GetDevice()->CopyDescriptors(1, &dstHandle, &dstRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void DescriptorTable::SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, E_SRV_REGISTER Register)
{
	D3D12_CPU_DESCRIPTOR_HANDLE dstHandle = GetCPUHandle(Register);

	uint32 dstRange = 1;
	uint32 srcRange = 1;
	DEVICE_OBJ->GetDevice()->CopyDescriptors(1, &dstHandle, &dstRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void DescriptorTable::CommitTable()
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = descHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += currentGroupIndex * groupSize;
	CMD_OBJ->GetCommandList()->SetGraphicsRootDescriptorTable(LITERAL::GLOBAL_CBUFFER_COUNT, handle);

	++currentGroupIndex;
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorTable::GetCPUHandle(E_CBV_REGISTER Register)
{
	return GetCPUHandle(static_cast<uint8>(Register));
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorTable::GetCPUHandle(E_SRV_REGISTER Register)
{
	return GetCPUHandle(static_cast<uint8>(Register));
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorTable::GetCPUHandle(uint8 Register)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = descHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += currentGroupIndex * groupSize;
	handle.ptr += (Register - LITERAL::GLOBAL_CBUFFER_COUNT) * handleSize;
	return handle;
}
