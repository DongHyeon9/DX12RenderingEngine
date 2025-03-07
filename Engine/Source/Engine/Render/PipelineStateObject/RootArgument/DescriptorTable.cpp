#include "DescriptorTable.h"

bool DescriptorTable::Init(Microsoft::WRL::ComPtr<ID3D12Device> Device, uint32 RootRarameterIdx, uint32 Count)
{
	LOG("디스크립터 힙 초기화 시작");

	groupCount = Count;
	rootParameterIdx = RootRarameterIdx;

	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.NumDescriptors = static_cast<UINT>(groupCount * registerCount);
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	HRESULT hr{};
	hr = Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descHeap));
	CHECK(SUCCEEDED(hr), "디스크립터 힙 생성 실패", false);
	LOG("디스크립터 힙 생성");

	handleSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	groupSize = handleSize * registerCount;

	LOG("디스크립터 힙 초기화 성공");
	return true;
}

void DescriptorTable::Clear()
{
	currentGroupIndex = 0;
}

void DescriptorTable::SetCBV(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_TABLE_CBV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle)
{
	D3D12_CPU_DESCRIPTOR_HANDLE dstHandle = descHeap->GetCPUDescriptorHandleForHeapStart();
	dstHandle.ptr += currentGroupIndex * groupSize;
	dstHandle.ptr += (static_cast<uint8>(Register) - rootParameterIdx) * handleSize;

	uint32 dstRange = 1;
	uint32 srcRange = 1;
	Device->CopyDescriptors(1, &dstHandle, &dstRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void DescriptorTable::SetSRV(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_TABLE_SRV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle)
{
	D3D12_CPU_DESCRIPTOR_HANDLE dstHandle = descHeap->GetCPUDescriptorHandleForHeapStart();
	dstHandle.ptr += currentGroupIndex * groupSize;
	dstHandle.ptr += (static_cast<uint8>(Register) - rootParameterIdx + srvOffset) * handleSize;

	uint32 dstRange = 1;
	uint32 srcRange = 1;
	Device->CopyDescriptors(1, &dstHandle, &dstRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void DescriptorTable::SetUAV(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_TABLE_UAV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle)
{
	D3D12_CPU_DESCRIPTOR_HANDLE dstHandle = descHeap->GetCPUDescriptorHandleForHeapStart();
	dstHandle.ptr += currentGroupIndex * groupSize;
	dstHandle.ptr += (static_cast<uint8>(Register) - rootParameterIdx + uavOffset) * handleSize;

	uint32 dstRange = 1;
	uint32 srcRange = 1;
	Device->CopyDescriptors(1, &dstHandle, &dstRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}


void DescriptorTable::CommitTable(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CmdList)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = descHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += currentGroupIndex * groupSize;
	CmdList->SetGraphicsRootDescriptorTable(rootParameterIdx, handle);

	++currentGroupIndex;
}