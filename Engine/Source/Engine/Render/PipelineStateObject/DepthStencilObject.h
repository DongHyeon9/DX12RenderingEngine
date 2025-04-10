#pragma once
#include "Header\EngineCore.h"

enum class E_DEPTH_STENCIL_STATE_FLAG : uint8
{
	DEFAULT = 0,

	END
};

class DepthStencilObject
{
public:

protected:

private:
	std::array<D3D12_DEPTH_STENCIL_DESC, static_cast<uint8>(E_DEPTH_STENCIL_STATE_FLAG::END)> depthStencilStates{};
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle{};

	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilBuffer{};
	uint32 dsvDescriptorSize{};
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap{};

public:
	bool Init();
	void ResetDepthStencilBuffer();
	bool CreateDepthStencilBuffer();

	FORCEINLINE D3D12_DEPTH_STENCIL_DESC GetDepthStencilState(E_DEPTH_STENCIL_STATE_FLAG DepthStencilFlag)const { return depthStencilStates[static_cast<uint8>(DepthStencilFlag)]; }
	FORCEINLINE D3D12_CPU_DESCRIPTOR_HANDLE GetDSVHandle() const { return dsvHandle; }
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12Resource> GetDepthStencilBuffer()const { return depthStencilBuffer; }

protected:

private:

};

