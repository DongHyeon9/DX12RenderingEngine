#include "RenderTarget.h"

#include "Manager\RenderManager.h"
#include "Engine\Render\DeviceObject.h"

bool RenderTarget::Init(std::vector<DXGI_FORMAT> Formats)
{
	LOG("���� Ÿ�� �ʱ�ȭ ����");

	Microsoft::WRL::ComPtr<ID3D12Device> device = DEVICE_OBJ->GetDevice();

	HRESULT hr{};
	descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	bufferFormats = std::move(Formats);

	const UINT bufferSize{ static_cast<UINT>(bufferFormats.size()) };
	renderTargets.resize(bufferSize);
	cpuHandles.resize(bufferSize);

	D3D12_DESCRIPTOR_HEAP_DESC renderTargetDesc{};
	renderTargetDesc.NumDescriptors = bufferSize;				//���� ������ŭ ��ũ���� ����
	renderTargetDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		//RTV Ÿ��
	renderTargetDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	//CPU ���� ���̹Ƿ� �÷��� ����
	renderTargetDesc.NodeMask = 0;								//�̱� GPU ȯ���̹Ƿ� 0

	hr = device->CreateDescriptorHeap(&renderTargetDesc, IID_PPV_ARGS(&descriptorHeap));
	CHECK(SUCCEEDED(hr), "���� Ÿ�� ���� ����", false);

	CD3DX12_CPU_DESCRIPTOR_HANDLE heapHandle{ descriptorHeap->GetCPUDescriptorHandleForHeapStart() };
	for (uint32 i = 0; i < bufferSize; ++i)
	{
		cpuHandles[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE{ heapHandle, static_cast<INT>(i * descriptorSize) };
	}

	LOG("���� Ÿ�� �ʱ�ȭ ����");
	return true;
}

void RenderTarget::ResetBuffer()
{
	for (auto buffer : renderTargets)
		buffer.Reset();
}
