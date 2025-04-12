#pragma once
#include "Header\EngineCore.h"

class DescriptorTable;
class ConstantBuffer;
class Texture;

enum class E_SAMPLER_TYPE : uint8
{
	WRAP = 0,
	CLAMP,

	END
};

class RootSignatureObject
{
public:

protected:

private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> signature{};
	std::array<D3D12_STATIC_SAMPLER_DESC, static_cast<uint8>(E_SAMPLER_TYPE::END)> samplers{};

	//TODO 이후 다중 시그니처를 사용하며 시그니처마다 디스크립터의 공간이 달라지면 vector로 변경될 수 도 있음
	std::shared_ptr<DescriptorTable> descriptorTable{};
	std::array<std::shared_ptr<ConstantBuffer>, ENUM_COUNT::CONSTANT_BUFFER_TYPE> constantBuffers{};

public:
	bool Init();
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12RootSignature> GetSignature()const { return signature; }
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorTable()const; 
	void PushData(E_CONSTANT_BUFFER_TYPE BufferType, void* Buffer, uint32 Size);
	void PushGlobalData(E_CONSTANT_BUFFER_TYPE BufferType, void* Buffer, uint32 Size);
	void PushTexture(E_SRV_REGISTER Register, std::shared_ptr<Texture> Texture);
	void PushTexture(E_SRV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE SrcHandle);
	void PushGlobalTexture(E_SRV_REGISTER Register, std::shared_ptr<Texture> Texture);
	void PushGlobalTexture(E_SRV_REGISTER Register, D3D12_GPU_VIRTUAL_ADDRESS SrcAddress);
	void Clear();
	void CommitDescriptor();

protected:

private:
	bool CreateSamplerDesc();
	bool CreateRootSignature();
	bool CreateConstantBuffer(E_CBV_REGISTER Register, E_CONSTANT_BUFFER_TYPE BufferType, uint32 BufferSize, uint32 Count);

};