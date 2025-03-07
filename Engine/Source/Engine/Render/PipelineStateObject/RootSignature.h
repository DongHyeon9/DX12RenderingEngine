#pragma once
#include "RootArgument\RootArguments.h"

enum class E_ROOT_SIGNATURE_FLAG : uint8
{
	DEFAULT = 0,

	END
};

class RootSignature
{
public:

protected:

private:
	//쉐이더와 그래픽스 파이프라인간의 리소스 바인딩 방식을 정의하는 객체
	std::array<Microsoft::WRL::ComPtr<ID3D12RootSignature>, static_cast<uint8>(E_ROOT_SIGNATURE_FLAG::END)> signatures{};
	//GPU에 데이터를 전달해 줄 객체
	std::array<RootArguments, static_cast<uint8>(E_ROOT_SIGNATURE_FLAG::END)> rootArguments{};

public:
	bool Init(Microsoft::WRL::ComPtr<ID3D12Device> Device);

	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature(E_ROOT_SIGNATURE_FLAG Flag)const { return signatures[static_cast<uint8>(Flag)]; }
	FORCEINLINE const RootArguments& GetRootArgument(E_ROOT_SIGNATURE_FLAG Flag)const { return rootArguments[static_cast<uint8>(Flag)]; }

	void SetUAV(E_ROOT_SIGNATURE_FLAG Flag, E_UAV_REGISTER Register, D3D12_GPU_VIRTUAL_ADDRESS GPUAddress);
	void SetSRV(E_ROOT_SIGNATURE_FLAG Flag, E_SRV_REGISTER Register, D3D12_GPU_VIRTUAL_ADDRESS GPUAddress);
	void SetCBV(E_ROOT_SIGNATURE_FLAG Flag, E_CBV_REGISTER Register, D3D12_GPU_VIRTUAL_ADDRESS GPUAddress);

	void SetUAVTable(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_ROOT_SIGNATURE_FLAG Flag, E_TABLE_UAV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle);
	void SetSRVTable(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_ROOT_SIGNATURE_FLAG Flag, E_TABLE_SRV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle);
	void SetCBVTable(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_ROOT_SIGNATURE_FLAG Flag, E_TABLE_CBV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle);

	void CommitData(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CmdList, E_ROOT_SIGNATURE_FLAG Flag);
	void ClearTable(E_ROOT_SIGNATURE_FLAG Flag);
	
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetTableDescriptorHeaps(E_ROOT_SIGNATURE_FLAG Flag)const { return rootArguments[static_cast<uint8>(Flag)].GetTableDescriptorHeap(); }

protected:

private:
	FORCEINLINE RootArguments& GetRootArgument(E_ROOT_SIGNATURE_FLAG Flag) { return rootArguments[static_cast<uint8>(Flag)]; }

};

