#pragma once
#include "Header\EngineCore.h"

class DescriptorTable
{
public:

protected:

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap{};
	uint64 handleSize{};
	uint64 groupSize{};
	uint64 groupCount{};

	uint32 currentGroupIndex{};

public:
	bool Init(uint32 Count);

	void Clear();
	void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, E_CBV_REGISTER Register);
	void SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, E_SRV_REGISTER Register);
	void CommitTable();

	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorTable()const { return descHeap; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(E_CBV_REGISTER Register);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(E_SRV_REGISTER Register);

protected:

private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint8 Register);
};

