#pragma once
#include "Header\EngineCore.h"

class DescriptorTable;
class ConstantBuffer;

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

	//TODO ���� ���� �ñ״�ó�� ����ϸ� �ñ״�ó���� ��ũ������ ������ �޶����� vector�� ����� �� �� ����
	std::shared_ptr<DescriptorTable> descriptorTable{};
	std::array<std::shared_ptr<ConstantBuffer>, ENUM_COUNT::CONSTANT_BUFFER_TYPE> constantBuffers{};

public:
	bool Init();
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12RootSignature> GetSignature()const { return signature; }

protected:

private:
	bool CreateSamplerDesc();
	bool CreateRootSignature();
	bool CreateConstantBuffer(E_CBV_REGISTER Register, E_CONSTANT_BUFFER_TYPE BufferType, uint32 BufferSize, uint32 Count);

};