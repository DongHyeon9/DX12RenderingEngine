#pragma once
#include "Header\EngineCoreMinimal.h"

class SwapChain
{
public:

protected:

private:
	//렌더링 영역을 설정하는 구조체
	//화면에서 어느 영역을 그릴지 결정하는 역할
	D3D12_VIEWPORT screenViewport{};
	//화면에서 특정 영역만 렌더링하도록 제한하는 구조체
	//뷰포트 안에서도 일부 영역만 그리도록 할 수 있음
	D3D12_RECT scissorRect{};

	//CPU에서 GPU로 전달된 렌더링 결과를 화면에 출력하는 역할
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain{};
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, LITERAL::SWAP_CHAIN_BUFFER_COUNT> swapChainBuffers{};
	std::array<D3D12_CPU_DESCRIPTOR_HANDLE, LITERAL::SWAP_CHAIN_BUFFER_COUNT> rtvHandles{};
	uint8 currentBackBufferIdx{};

	uint32 rtvDescriptorSize{};

	//GPU 리소스(텍스처, 버퍼, 샘플러 등)를 참조하는 디스크립터(Descriptor)를 저장하는 힙(Heap)
	//DX11에서는 자동으로 바인딩되었지만, DX12에서는 개발자가 직접 관리
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap{};

public:
	bool Init(Microsoft::WRL::ComPtr<ID3D12Device> Device, Microsoft::WRL::ComPtr<IDXGIFactory4> Factory, Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQueue);
	void ResetSwapChain();
	bool CreateSwapChainBuffer(Microsoft::WRL::ComPtr<ID3D12Device> Device);
	void SetScreenViewport();
	void SetScissorRect();
	void Present();
	void SwapIndex();

	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12Resource> GetBackBuffer()const { return swapChainBuffers[currentBackBufferIdx]; }
	FORCEINLINE D3D12_CPU_DESCRIPTOR_HANDLE GetBackBufferRTV()const { return rtvHandles[currentBackBufferIdx]; }
	FORCEINLINE D3D12_VIEWPORT& GetViewport() { return screenViewport; }
	FORCEINLINE D3D12_RECT& GetScissorRect() { return scissorRect; }
protected:

private:

};

