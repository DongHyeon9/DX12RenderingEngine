#include "RootSignature.h"

bool RootSignature::Init(Microsoft::WRL::ComPtr<ID3D12Device> Device)
{
	LOG("루트 시그니처 초기화 시작");
	uint32 idx{ static_cast<uint8>(E_ROOT_SIGNATURE_FLAG::DEFAULT) };

	RootArgumentDesc desc{};
	desc.cbvCount = 3;
	desc.srvCount = 0;
	desc.uavCount = 0;
	desc.cbvTableNum = 256;
	desc.srvTableNum = 256;

	rootArguments[idx].Init(Device, IID_PPV_ARGS(&signatures[idx]), desc);

	LOG("루트 시그니처 초기화 성공");
	return true;
}

void RootSignature::SetUAV(E_ROOT_SIGNATURE_FLAG Flag, E_UAV_REGISTER Register, D3D12_GPU_VIRTUAL_ADDRESS GPUAddress)
{
	GetRootArgument(Flag).SetUAV(Register, GPUAddress);
}

void RootSignature::SetSRV(E_ROOT_SIGNATURE_FLAG Flag, E_SRV_REGISTER Register, D3D12_GPU_VIRTUAL_ADDRESS GPUAddress)
{
	GetRootArgument(Flag).SetSRV(Register, GPUAddress);
}

void RootSignature::SetCBV(E_ROOT_SIGNATURE_FLAG Flag, E_CBV_REGISTER Register, D3D12_GPU_VIRTUAL_ADDRESS GPUAddress)
{
	GetRootArgument(Flag).SetCBV(Register, GPUAddress);
}

void RootSignature::SetUAVTable(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_ROOT_SIGNATURE_FLAG Flag, E_TABLE_UAV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle)
{
	GetRootArgument(Flag).SetUAVTable(Device, Register, srcHandle);
}

void RootSignature::SetSRVTable(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_ROOT_SIGNATURE_FLAG Flag, E_TABLE_SRV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle)
{
	GetRootArgument(Flag).SetSRVTable(Device, Register, srcHandle);
}

void RootSignature::SetCBVTable(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_ROOT_SIGNATURE_FLAG Flag, E_TABLE_CBV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle)
{
	GetRootArgument(Flag).SetCBVTable(Device, Register, srcHandle);
}

void RootSignature::CommitData(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CmdList, E_ROOT_SIGNATURE_FLAG Flag)
{
	GetRootArgument(Flag).CommitData(CmdList);
}

void RootSignature::ClearTable(E_ROOT_SIGNATURE_FLAG Flag)
{
	GetRootArgument(Flag).Clear();
}
