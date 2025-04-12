#pragma once
#include "Resource.h"

class Texture : public Resource
{
public:

protected:

private:
	DirectX::ScratchImage image{};
	Microsoft::WRL::ComPtr<ID3D12Resource> texture2D{};

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap{};
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle{};

public:
	Texture() :Resource(EngineUtil::Path::GetTexturesDir()) {}
	bool Load(FString FileName)override;
	bool CreateView(D3D12_SRV_DIMENSION TextureDimension = D3D12_SRV_DIMENSION_TEXTURE2D);
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap()const { return srvHeap; }
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12Resource> GetTexture()const { return texture2D; }
	FORCEINLINE D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandle()const { return srvHandle; }

protected:

private:

};

