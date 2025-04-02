#pragma once
#include "Resource.h"

class Texture : public Resource<Microsoft::WRL::ComPtr<ID3D12Resource>>
{
public:

protected:

private:
	DirectX::ScratchImage image{};
	Microsoft::WRL::ComPtr<ID3D12Resource> texture2D{};

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap{};
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle{};

public:
	Texture() :Resource<Microsoft::WRL::ComPtr<ID3D12Resource>>(EngineUtil::Path::GetTexturesDir()) {}
	bool Load(FString FileName)override;

	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12Resource> GetResource()const override { return texture2D; }
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap()const { return srvHeap; }

protected:

private:

};

