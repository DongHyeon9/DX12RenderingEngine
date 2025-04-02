#pragma once
#include "Header\EngineCore.h"

class SwapChain
{
public:

protected:

private:
	D3D12_VIEWPORT screenViewport{};
	D3D12_RECT scissorRect{};

	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain{};
	uint8 currentBackBufferIdx{};

	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, LITERAL::SWAP_CHAIN_BUFFER_COUNT> swapChainBuffers{};
	std::array<D3D12_CPU_DESCRIPTOR_HANDLE, LITERAL::SWAP_CHAIN_BUFFER_COUNT> rtvHandles{};

	uint32 rtvDescriptorSize{};

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap{};

public:
	bool Init(Microsoft::WRL::ComPtr<ID3D12Device> Device, Microsoft::WRL::ComPtr<IDXGIFactory4> Factory, Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQueue);
	void ResetSwapChain();
	bool CreateSwapChainBuffer(Microsoft::WRL::ComPtr<ID3D12Device> Device);
	void SetScreenViewport();
	void SetScissorRect();
	void Present();
	void SwapIndex();

	FORCEINLINE Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain()const { return swapChain; }
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12Resource> GetBackBuffer()const { return swapChainBuffers[currentBackBufferIdx]; }
	FORCEINLINE D3D12_CPU_DESCRIPTOR_HANDLE GetBackBufferRTV()const { return rtvHandles[currentBackBufferIdx]; }
	FORCEINLINE D3D12_VIEWPORT& GetViewport() { return screenViewport; }
	FORCEINLINE D3D12_RECT& GetScissorRect() { return scissorRect; }
protected:

private:

};

