#include "SwapChainObject.h"

#include "Engine\Engine.h"
#include "Manager\RenderManager.h"

#include "Engine\Render\DeviceObject.h"
#include "Engine\Render\CommandObject.h"

bool SwapChainObject::Init()
{
	LOG("����ü�� �ʱ�ȭ ����");

	EngineSetting engineSetting = GEngine->GetEngineSetting();
	HRESULT hr{};

	swapChain.Reset();

	DXGI_SWAP_CHAIN_DESC swapCahinDesc{};
	swapCahinDesc.BufferDesc.Width = static_cast<UINT>(engineSetting.resolution.x);	//�ʺ� (�ػ� x ��)
	swapCahinDesc.BufferDesc.Height = static_cast<UINT>(engineSetting.resolution.y);	//���� (�ػ� y ��)
	swapCahinDesc.BufferDesc.RefreshRate.Numerator = 60;		//ȭ�� �ֻ��� 60Hz ���� (�ʴ� 60�� ����)
	swapCahinDesc.BufferDesc.RefreshRate.Denominator = 1;	//�и� 1�� �����Ͽ� ��Ȯ�� 60Hz ����
	swapCahinDesc.BufferDesc.Format = LITERAL::BACK_BUFFER_FORMAT;		//�� ������ �ȼ� ����(�Ϲ������� DXGI_FORMAT_R8G8B8A8_UNORM ���)
	swapCahinDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	//��ĵ���� ������ �������� ���� (*1)
	swapCahinDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					//ȭ�� �����ϸ� �ɼ��� �������� ���� (�⺻ ��) (*2)
	swapCahinDesc.SampleDesc.Count = engineSetting.state4XMSAA ? 4 : 1;											//MSAA�� ���� ������ ����
	swapCahinDesc.SampleDesc.Quality = engineSetting.state4XMSAA ? (engineSetting.quality4XMSAA - 1) : 0;		//MSAA�� ǰ�� ����(0�̸� �⺻ ��)
	swapCahinDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		//���� ü���� ���۰� ���� Ÿ�� ��¿����� ����
	swapCahinDesc.BufferCount = LITERAL::SWAP_CHAIN_BUFFER_COUNT;		//�� ���� ����
	swapCahinDesc.OutputWindow = engineSetting.hWnd;		//����� ������ �ڵ�
	swapCahinDesc.Windowed = true;							//â ���(true) �Ǵ� ��ü ȭ�� ���(false)
	swapCahinDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		//DirectX 12���� ���� ����Ǵ� ���� ��� (*3)
	swapCahinDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	//��ü ȭ��� â ��� ��ȯ�� ���

	hr = DEVICE_OBJ->GetFactory()->CreateSwapChain(
		CMD_OBJ->GetCommandQueue().Get(),		//GPU ��� ������ ����ϴ� Ŀ�ǵ� ť�� ����
		&swapCahinDesc,
		swapChain.GetAddressOf());

	rtvDescriptorSize = DEVICE_OBJ->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CHECK(SUCCEEDED(hr), "����ü�� ���� ����", false);

	LOG("����ü�� �ʱ�ȭ ����");
	return true;
}

void SwapChainObject::ResetSwapChain()
{
	for (auto swapChainBuffer : swapChainBuffers)
		swapChainBuffer.Reset();
}

bool SwapChainObject::CreateSwapChainBuffer()
{
	EngineSetting engineSetting = GEngine->GetEngineSetting();
	HRESULT hr{};

	Microsoft::WRL::ComPtr<ID3D12Device> device = DEVICE_OBJ->GetDevice();

	rtvHeap.Reset();

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.NumDescriptors = LITERAL::SWAP_CHAIN_BUFFER_COUNT;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;

	CHECK(SUCCEEDED(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(rtvHeap.GetAddressOf()))), "RTV Heap ���� ����", false);

	//���ο� �ػ󵵿� �°� ���� ü���� �� ���� ũ�⸦ ����
	hr = swapChain->ResizeBuffers(
		LITERAL::SWAP_CHAIN_BUFFER_COUNT,		//���� ����
		static_cast<UINT>(engineSetting.resolution.x),
		static_cast<UINT>(engineSetting.resolution.y),
		LITERAL::BACK_BUFFER_FORMAT,					//���� ����
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);		//����ü�� �÷���
	CHECK(SUCCEEDED(hr), "���� �������� ����", false);
	currentBackBufferIdx = 0;
	
	//���ο� �� ���� �Ҵ� �� RTV ����
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle{ rtvHeap->GetCPUDescriptorHandleForHeapStart() };
	for (uint32 i = 0; i < LITERAL::SWAP_CHAIN_BUFFER_COUNT; ++i)
	{
		//���� ��������� �� ����(���� ü�� ����)�� ������
		hr = swapChain->GetBuffer(
			i,	//���� ü�� ���� �ε���
			IID_PPV_ARGS(&swapChainBuffers[i]));
		CHECK(SUCCEEDED(hr), "����ü�� ���� �������� ����", false);
		//RTV(���� Ÿ�� ��)�� �����Ͽ� �� ���۸� ���� Ÿ������ ��� �����ϰ� ����
		rtvHandles[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE{ rtvHeapHandle, static_cast<INT>(i * rtvDescriptorSize) };
		device->CreateRenderTargetView(
			swapChainBuffers[i].Get(),
			nullptr,		//�⺻ RTV ���� ���
			rtvHandles[i]);	//RTV�� ����� ��ũ���� ���� ��ġ
	}

	return true;
}

void SwapChainObject::SetScreenViewport()
{
	EngineSetting engineSetting = GEngine->GetEngineSetting();
	//���ο� �ػ󵵿� �°� ����Ʈ(Viewport) �� ����(Rectangle)�� ������Ʈ
	screenViewport.TopLeftX = 0;
	screenViewport.TopLeftY = 0;
	screenViewport.Width = engineSetting.resolution.x;
	screenViewport.Height = engineSetting.resolution.y;
	screenViewport.MinDepth = 0.0f;
	screenViewport.MaxDepth = 1.0f;
}

void SwapChainObject::SetScissorRect()
{
	EngineSetting engineSetting = GEngine->GetEngineSetting();
	scissorRect = { 0, 0, static_cast<int32>(engineSetting.resolution.x), static_cast<int32>(engineSetting.resolution.y) };
}

void SwapChainObject::Present()
{
	swapChain->Present(0, 0);
}

void SwapChainObject::SwapIndex()
{
	currentBackBufferIdx = (currentBackBufferIdx + 1) % LITERAL::SWAP_CHAIN_BUFFER_COUNT;
}
