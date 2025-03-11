#include "SwapChain.h"
#include "Engine\Engine.h"

bool SwapChain::Init(Microsoft::WRL::ComPtr<ID3D12Device> Device, Microsoft::WRL::ComPtr<IDXGIFactory4> Factory, Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQueue)
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

	hr = Factory->CreateSwapChain(
		CommandQueue.Get(),		//GPU ��� ������ ����ϴ� Ŀ�ǵ� ť�� ����
		&swapCahinDesc,
		swapChain.GetAddressOf());

	CHECK(SUCCEEDED(hr), "����ü�� ���� ����", false);

	rtvDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	LOG("���� Ÿ�� �� ���� ����");

	D3D12_DESCRIPTOR_HEAP_DESC renderTargetDesc{};
	renderTargetDesc.NumDescriptors = LITERAL::SWAP_CHAIN_BUFFER_COUNT;	//���� ������ŭ ��ũ���� ����
	renderTargetDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;				//RTV Ÿ��
	renderTargetDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;			//CPU ���� ���̹Ƿ� �÷��� ����
	renderTargetDesc.NodeMask = 0;										//�̱� GPU ȯ���̹Ƿ� 0

	hr = Device->CreateDescriptorHeap(&renderTargetDesc, IID_PPV_ARGS(&rtvHeap));
	CHECK(SUCCEEDED(hr), "���� Ÿ�� ���� ����", false);
	LOG("���� Ÿ�� ���� ����");

	LOG("����ü�� �ʱ�ȭ ����");
	return true;
}

void SwapChain::ResetSwapChain()
{
	for (auto swapChainBuffer : swapChainBuffers)
		swapChainBuffer.Reset();
}

bool SwapChain::CreateSwapChainBuffer(Microsoft::WRL::ComPtr<ID3D12Device> Device)
{
	EngineSetting engineSetting = GEngine->GetEngineSetting();
	HRESULT hr{};

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
		Device->CreateRenderTargetView(
			swapChainBuffers[i].Get(),
			nullptr,		//�⺻ RTV ���� ���
			rtvHeapHandle);	//RTV�� ����� ��ũ���� ���� ��ġ
		//���� RTV ��ũ���� ��ġ�� �̵�
		rtvHeapHandle.Offset(1, rtvDescriptorSize);
	}

	UINT rtvHeapSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	for (uint8 i = 0; i < LITERAL::SWAP_CHAIN_BUFFER_COUNT; ++i)
	{
		rtvHandles[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE{ rtvHeapBegin, static_cast<INT>(i * rtvHeapSize) };
		Device->CreateRenderTargetView(swapChainBuffers[i].Get(), nullptr, rtvHandles[i]);
	}

	return true;
}

void SwapChain::SetScreenViewport()
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

void SwapChain::SetScissorRect()
{
	EngineSetting engineSetting = GEngine->GetEngineSetting();
	scissorRect = { 0, 0, static_cast<int32>(engineSetting.resolution.x), static_cast<int32>(engineSetting.resolution.y) };
}

void SwapChain::Present()
{
	swapChain->Present(0, 0);
}

void SwapChain::SwapIndex()
{
	currentBackBufferIdx = (currentBackBufferIdx + 1) % LITERAL::SWAP_CHAIN_BUFFER_COUNT;
}
