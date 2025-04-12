#pragma once
#include "Header\EngineCore.h"

class ConstantBuffer
{
public:

protected:

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> cbvBuffer{};
	BYTE* mappedBuffer{};
	uint32 elementSize{};
	uint32 elementCount{};
	uint32 currentIndex{};

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> cbvHeap{};
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandleBegin{};
	uint32 handleIncreamentSize{};

	E_CBV_REGISTER cbvRegister{};

public:
	virtual ~ConstantBuffer();

	bool Init(E_CBV_REGISTER Register, uint32 Size, uint32 Count);
	void Clear();
	D3D12_CPU_DESCRIPTOR_HANDLE PushData(void* Buffer, uint32 Size);
	void PushGlobalData(void* Buffer, uint32 Size);
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress(uint32 Index);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32 Index);

protected:

private:
	bool CreateBuffer();
	bool CreateView();
};

