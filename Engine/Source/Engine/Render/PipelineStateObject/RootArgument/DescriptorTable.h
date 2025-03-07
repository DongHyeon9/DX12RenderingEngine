#pragma once
#include "Header\EngineCoreMinimal.h"

class DescriptorTable
{
public:

protected:

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap{};
	uint64 handleSize{};
	uint64 groupSize{};
	uint64 groupCount{};

	uint32 registerCount{ ENUM_COUNT::TABLE_CBV_REGISTER + ENUM_COUNT::TABLE_SRV_REGISTER + ENUM_COUNT::TABLE_UAV_REGISTER };
	uint32 srvOffset{ ENUM_COUNT::TABLE_CBV_REGISTER };
	uint32 uavOffset{ ENUM_COUNT::TABLE_CBV_REGISTER + ENUM_COUNT::TABLE_SRV_REGISTER };
	uint8 rootParameterIdx{};

	uint32 currentGroupIndex{};

public:
	bool Init(Microsoft::WRL::ComPtr<ID3D12Device> Device, uint32 RootRarameterIdx, uint32 Count);

	void Clear();
	void SetCBV(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_TABLE_CBV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle);
	void SetSRV(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_TABLE_SRV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle);
	void SetUAV(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_TABLE_UAV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle);
	void CommitTable(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CmdList);

	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap()const { return descHeap; }

protected:

private:

};