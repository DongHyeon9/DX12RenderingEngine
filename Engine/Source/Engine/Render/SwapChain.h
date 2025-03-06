#pragma once
#include "Header\EngineCoreMinimal.h"

class SwapChain
{
public:

protected:

private:
	//������ ������ �����ϴ� ����ü
	//ȭ�鿡�� ��� ������ �׸��� �����ϴ� ����
	D3D12_VIEWPORT screenViewport{};
	//ȭ�鿡�� Ư�� ������ �������ϵ��� �����ϴ� ����ü
	//����Ʈ �ȿ����� �Ϻ� ������ �׸����� �� �� ����
	D3D12_RECT scissorRect{};

	//CPU���� GPU�� ���޵� ������ ����� ȭ�鿡 ����ϴ� ����
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain{};
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, LITERAL::SWAP_CHAIN_BUFFER_COUNT> swapChainBuffers{};
	std::array<D3D12_CPU_DESCRIPTOR_HANDLE, LITERAL::SWAP_CHAIN_BUFFER_COUNT> rtvHandles{};
	uint8 currentBackBufferIdx{};

	uint32 rtvDescriptorSize{};

	//GPU ���ҽ�(�ؽ�ó, ����, ���÷� ��)�� �����ϴ� ��ũ����(Descriptor)�� �����ϴ� ��(Heap)
	//DX11������ �ڵ����� ���ε��Ǿ�����, DX12������ �����ڰ� ���� ����
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

