#include "Engine.h"
#include "AppBase.h"

#include "Manager\CameraManager.h"
#include "Manager\GeometryManager.h"
#include "Manager\InputManager.h"
#include "Manager\TimerManager.h"

bool Engine::Init(const EngineDesc& EngineDescription)
{
	app = EngineDescription.app;
	engineSetting.hInstance = EngineDescription.hInstance;
	engineSetting.resolution = Vector2(static_cast<float>(EngineDescription.width), static_cast<float>(EngineDescription.height));

	CHECK(app, "app is nullptr", false);
	CHECK(engineSetting.hInstance, "hInstance is nullptr", false);

	LOG("Initlaize Process");

	//���� ����
#ifdef UNICODE
	_wsetlocale(LC_ALL, TEXT("korean"));
	LOG("���� : �ѱ�");
#endif

	//������ �ʱ�ȭ
	CHECK(InitWindow(), "������ �ʱ�ȭ ����", false);

	//DirectX �ʱ�ȭ
	CHECK(InitDirectX(), "DirectX �ʱ�ȭ ����", false);

	//�Ŵ��� �ʱ�ȭ
	CHECK(InitManager(), "�Ŵ��� �ʱ�ȭ ����", false);

	LOG("���� �ʱ�ȭ ����");
	return true;
}

WPARAM Engine::Run()
{
	LOG("���α׷� ����");

	//�� �ʱ�ȭ
	CHECK(app->Init(), "�� �ʱ�ȭ ����", false);

	commandObject->FlushCommandQueue();

	//���α׷� ���� ����
	MSG msg{};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		EngineUpdate();
	}
	return msg.wParam;
}

bool Engine::InitWindow()
{
	LOG("������ �ʱ�ȭ ����");

	CHECK(EngineRegisterClass(), "������ Ŭ���� ��� ����", false);
	CHECK(InitInstance(SW_SHOWNORMAL), "������ â ���� ����", false);

	LOG("������ �ʱ�ȭ ����");
	return true;
}

bool Engine::InitDirectX()
{
	LOG("DX12 �ʱ�ȭ ����");

	device = std::make_unique<Device>();
	commandObject = std::make_unique<CommandObject>();
	swapChain = std::make_unique<SwapChain>();
	pipelineStateObject = std::make_unique<PipelineStateObject>();

	CHECK(device->Init(), "����̽� �ʱ�ȭ ����", false);
	CHECK(commandObject->Init(device->GetDevice()), "Ŀ�ǵ� ������Ʈ �ʱ�ȭ ����", false);
	CHECK(swapChain->Init(device->GetDevice(), device->GetFactory(), commandObject->GetCommandQueue()), "����ü�� �ʱ�ȭ ����", false);
	CHECK(pipelineStateObject->Init(device->GetDevice()), "���������� ������Ʈ �ʱ�ȭ ����", false);
	
	CHECK(ResizeWindow(), "������ ������ ���� ����", false);

	LOG("DX12 �ʱ�ȭ ����");
	return true;
}

bool Engine::InitManager()
{
	LOG("�Ŵ��� �ʱ�ȭ ����");

	CHECK(CameraManager::GetInstance()->Init(), "ī�޶� �Ŵ��� �ʱ�ȭ ����", false);
	CHECK(GeometryManager::GetInstance()->Init(), "������Ʈ�� �Ŵ��� �ʱ�ȭ ����", false);
	CHECK(InputManager::GetInstance()->Init(), "��ǲ �Ŵ��� �ʱ�ȭ ����", false);
	CHECK(TimerManager::GetInstance()->Init(), "Ÿ�̸� �Ŵ��� �ʱ�ȭ ����", false);

	LOG("�Ŵ��� �ʱ�ȭ ����");
	return true;
}

ATOM Engine::EngineRegisterClass()
{
	LOG("������ Ŭ���� ���");
	WNDCLASSEXW wcex{};

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = engineSetting.hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = engineSetting.engineName.c_str();
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex);
}

BOOL Engine::InitInstance(int CmdShow)
{
	LONG width = static_cast<LONG>(engineSetting.resolution.x);
	LONG height = static_cast<LONG>(engineSetting.resolution.y);
	LOG("������ ũ�� ���� : %d * %d", width, height);
	RECT windowRect = { 0, 0, width, height };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	engineSetting.hWnd = CreateWindow(
		engineSetting.engineName.c_str(),
		engineSetting.engineName.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, engineSetting.hInstance, nullptr);

	if (engineSetting.hWnd == NULL)
		return FALSE;

	ShowWindow(engineSetting.hWnd, CmdShow);
	UpdateWindow(engineSetting.hWnd);

	return TRUE;
}

bool Engine::InitEngineSetting()
{
	//4X MSAA(Multi Sample Anti-Aliasing) ǰ�� ���� Ȯ��
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = LITERAL::BACK_BUFFER_FORMAT;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;

	HRESULT hr{};
	hr = device->GetDevice()->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels));
	CHECK(SUCCEEDED(hr), "4X MSAA ǰ�� Ȯ�� ����", false);

	engineSetting.quality4XMSAA = msQualityLevels.NumQualityLevels;
	CHECK(engineSetting.quality4XMSAA > 0, "����ġ ���� 4X MSAA ǰ��", false);
	LOG("4X MSAA ǰ�� : %d", engineSetting.quality4XMSAA);

	return true;
}

void Engine::EngineUpdate()
{
	const float deltaTime{ TimerManager::GetInstance()->Update() };
	InputManager::GetInstance()->Update();
	CameraManager::GetInstance()->Update(deltaTime);

	Update(deltaTime);
	Render();
}

void Engine::Update(float DeltaTime)
{
	app->Update(DeltaTime);

	app->LateUpdate(DeltaTime);
}

void Engine::RenderBegin()
{
	HRESULT hr{};
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList = commandObject->GetCommandList();
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAlloc = commandObject->GetCommandAllocator();
	hr = cmdAlloc->Reset();
	hr = cmdList->Reset(cmdAlloc.Get(), nullptr);

	//������ۿ� �ĸ������ ��ü
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		swapChain->GetBackBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	cmdList->SetGraphicsRootSignature(pipelineStateObject->GetRootSignature()->GetRootSignature(E_ROOT_SIGNATURE_FLAG::DEFAULT).Get());
	pipelineStateObject->GetRootSignature()->ClearTable(E_ROOT_SIGNATURE_FLAG::DEFAULT);

	ID3D12DescriptorHeap* tableDescriptorHeap = pipelineStateObject->GetRootSignature()->GetTableDescriptorHeaps(E_ROOT_SIGNATURE_FLAG::DEFAULT).Get();

	if (tableDescriptorHeap != nullptr)
	{
		cmdList->SetDescriptorHeaps(1, &tableDescriptorHeap);
	}

	cmdList->ResourceBarrier(1, &barrier);

	cmdList->RSSetViewports(1, &swapChain->GetViewport());
	cmdList->RSSetScissorRects(1, &swapChain->GetScissorRect());

	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = swapChain->GetBackBufferRTV();
	cmdList->ClearRenderTargetView(backBufferView, GLOBAL::CLEAR_COLOR, 0, nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = pipelineStateObject->GetDepthStencilObject()->GetDSVHandle();
	cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, &depthStencilView);
	cmdList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0, 0, 0, nullptr);

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pso = pipelineStateObject->GetPipelineStateObject(E_RENDERING_FLAG::DEFAULT);

	cmdList->SetPipelineState(pso.Get());
}

void Engine::Render()
{
	RenderBegin();

	app->Render();

	RenderEnd();
}

void Engine::RenderEnd()
{
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList = commandObject->GetCommandList();

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		swapChain->GetBackBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	cmdList->ResourceBarrier(1, &barrier);
	HRESULT hr = cmdList->Close();
	ID3D12CommandList* cmdlistArr[]{ cmdList.Get() };
	commandObject->GetCommandQueue()->ExecuteCommandLists(_countof(cmdlistArr), cmdlistArr);
	swapChain->Present();

	commandObject->FlushCommandQueue();

	swapChain->SwapIndex();
}

bool Engine::ResizeWindow()
{
	LOG("����ü�� �������� ����");

	//���� Ŀ�ǵ尡 ��� ���� ������ ���
	CHECK(commandObject->FlushCommandQueue(), "Ŀ�ǵ� ť ���� ����", false);

	HRESULT hr{};

	CHECK(commandObject->ResetCmdList(), "Ŀ�ǵ� ����Ʈ ���� ����", false);

	CHECK(SUCCEEDED(hr), "Ŀ�ǵ� ����Ʈ ���� ����", false);

	//������ ���� ü�� ���ۿ� ���� ���ٽ� ���۸� ����
	swapChain->ResetSwapChain();
	pipelineStateObject->GetDepthStencilObject()->ResetDepthStencilBuffer();

	CHECK(swapChain->CreateSwapChainBuffer(device->GetDevice()), "����ü�� ���� ���� ����", false);
	CHECK(pipelineStateObject->GetDepthStencilObject()->CreateDepthStencilBuffer(device->GetDevice()), "���� ���ٽ� ���� ���� ����", false);
	
	//���� ���ٽ� ���۸� D3D12_RESOURCE_STATE_DEPTH_WRITE ���·� �����Ͽ� ������ �� ���� ���� �����ϵ��� ����
	D3D12_RESOURCE_BARRIER barrier{ CD3DX12_RESOURCE_BARRIER::Transition(
		pipelineStateObject->GetDepthStencilObject()->GetDepthStencilBuffer().Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_DEPTH_WRITE) };
	commandObject->GetCommandList()->ResourceBarrier(1, &barrier);
	commandObject->ExecuteCommandList();

	CHECK(commandObject->FlushCommandQueue(), "Ŀ�ǵ� ť ���� ����", false);

	swapChain->SetScreenViewport();
	swapChain->SetScissorRect();

	LOG("����ü�� �������� ����");
	return true;
}

//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);

LRESULT WndProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(WM_QUIT);
		break;
	case WM_COMMAND:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		break;
	default:
		return DefWindowProc(Handle, Message, wParam, lParam);
	}

	//ImGui_ImplWin32_WndProcHandler(Handle, Message, wParam, lParam);
	return GEngine->GetApp()->WndProc(Handle, Message, wParam, lParam);
}