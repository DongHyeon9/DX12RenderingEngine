#pragma once
#include "DescriptorTable.h"

struct RootArgumentDesc
{
	uint32 cbvCount{};
	uint32 srvCount{};
	uint32 uavCount{};
	uint32 tableCount{};
};

class RootArguments
{
public:

protected:

private:
	//텍스처 샘플링시 사용할 필터링 방식 및 텍스처 좌표 래핑 방식
	std::vector<D3D12_STATIC_SAMPLER_DESC> samplerDescs{};

	DescriptorTable table{};
	std::array<D3D12_GPU_VIRTUAL_ADDRESS, static_cast<uint8>(E_CBV_REGISTER::END)> cbvRegisters{};
	std::array<D3D12_GPU_VIRTUAL_ADDRESS, static_cast<uint8>(E_SRV_REGISTER::END)> srvRegisters{};
	std::array<D3D12_GPU_VIRTUAL_ADDRESS, static_cast<uint8>(E_UAV_REGISTER::END)> uavRegisters{};

	RootArgumentDesc desc{};

public:
	bool Init(Microsoft::WRL::ComPtr<ID3D12Device> Device, const IID& Riid, void** ppvRootSignature, const RootArgumentDesc& Desc);

	void SetUAV(E_UAV_REGISTER Register, D3D12_GPU_VIRTUAL_ADDRESS GPUAddress);
	void SetSRV(E_SRV_REGISTER Register, D3D12_GPU_VIRTUAL_ADDRESS GPUAddress);
	void SetCBV(E_CBV_REGISTER Register, D3D12_GPU_VIRTUAL_ADDRESS GPUAddress);

	void SetUAVTable(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_TABLE_UAV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle);
	void SetSRVTable(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_TABLE_SRV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle);
	void SetCBVTable(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_TABLE_CBV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle);

	void CommitData(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CmdList);
	void Clear();

	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetTableDescriptorHeap()const { return table.GetDescriptorHeap(); }

protected:

private:
	bool CreateSamplerDesc();
	bool CreateRootSignature(Microsoft::WRL::ComPtr<ID3D12Device> Deivce, const IID& Riid, void** ppvRootSignature);
};
