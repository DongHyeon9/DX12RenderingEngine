#pragma once
#include "Header\EngineCore.h"

class RenderTarget
{
public:

protected:

private:
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> renderTargets{};
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> cpuHandles{};
	std::vector<DXGI_FORMAT> bufferFormats{};

	uint32 descriptorSize{};
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap{};

public:
	bool Init(Microsoft::WRL::ComPtr<ID3D12Device> Device, std::vector<DXGI_FORMAT> Formats);
	void ResetBuffer();

protected:

private:

};

