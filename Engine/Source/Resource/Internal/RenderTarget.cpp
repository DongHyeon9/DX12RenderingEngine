#include "RenderTarget.h"

#include "Manager\RenderManager.h"
#include "Engine\Render\DeviceObject.h"

bool RenderTarget::Init(std::vector<DXGI_FORMAT> Formats)
{
	LOG("렌더 타겟 초기화 시작");

	Microsoft::WRL::ComPtr<ID3D12Device> device = DEVICE_OBJ->GetDevice();

	HRESULT hr{};
	descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	bufferFormats = std::move(Formats);

	const UINT bufferSize{ static_cast<UINT>(bufferFormats.size()) };
	renderTargets.resize(bufferSize);
	cpuHandles.resize(bufferSize);

	D3D12_DESCRIPTOR_HEAP_DESC renderTargetDesc{};
	renderTargetDesc.NumDescriptors = bufferSize;				//버퍼 개수만큼 디스크립터 생성
	renderTargetDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		//RTV 타입
	renderTargetDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	//CPU 전용 힙이므로 플래그 없음
	renderTargetDesc.NodeMask = 0;								//싱글 GPU 환경이므로 0

	hr = device->CreateDescriptorHeap(&renderTargetDesc, IID_PPV_ARGS(&descriptorHeap));
	CHECK(SUCCEEDED(hr), "렌더 타겟 생성 실패", false);

	CD3DX12_CPU_DESCRIPTOR_HANDLE heapHandle{ descriptorHeap->GetCPUDescriptorHandleForHeapStart() };
	for (uint32 i = 0; i < bufferSize; ++i)
	{
		cpuHandles[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE{ heapHandle, static_cast<INT>(i * descriptorSize) };
	}

	LOG("렌더 타겟 초기화 성공");
	return true;
}

void RenderTarget::ResetBuffer()
{
	for (auto buffer : renderTargets)
		buffer.Reset();
}
