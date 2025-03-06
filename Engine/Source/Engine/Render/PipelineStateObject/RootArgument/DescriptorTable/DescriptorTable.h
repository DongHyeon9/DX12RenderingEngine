#pragma once
#include "Header\EngineCoreMinimal.h"

template <typename _RegisterType>
concept IsValidRegisterType =
std::same_as<_RegisterType, E_TABLE_CBV_REGISTER> ||
std::same_as<_RegisterType, E_TABLE_SRV_REGISTER> ||
std::same_as<_RegisterType, E_TABLE_UAV_REGISTER>;

template<IsValidRegisterType _RegisterType>
class DescriptorTable
{
public:

protected:

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap{};
	uint64 handleSize{};
	uint64 groupSize{};
	uint64 groupCount{};

	uint8 registerCount{ static_cast<uint8>(_RegisterType::END) };
	uint8 rootParameterIdx{};

	uint32 currentGroupIndex{};

public:
	bool Init(Microsoft::WRL::ComPtr<ID3D12Device> Device, uint32 RootRarameterIdx, uint32 Count);

	void Clear();
	void SetData(Microsoft::WRL::ComPtr<ID3D12Device> Device, _RegisterType Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle);
	void CommitTable(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CmdList);

	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap()const { return descHeap; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(_RegisterType Register);

protected:

private:

};

#include "DescriptorTable.inl"
