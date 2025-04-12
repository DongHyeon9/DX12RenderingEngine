#include "Engine.h"
#include "AppBase.h"

#include "Manager\RenderManager.h"
#include "Manager\CameraManager.h"
#include "Manager\GeometryManager.h"
#include "Manager\InputManager.h"
#include "Manager\TimerManager.h"
#include "Manager\ResourceManager.h"

#include "Engine\Render\DeviceObject.h"
#include "Engine\Render\CommandObject.h"
#include "Engine\Render\PipelineStateObject.h"
#include "Engine\Render\SwapChainObject.h"

#include "Resource\External\Texture.h"

bool Engine::Init(const EngineDesc& EngineDescription)
{
	app = EngineDescription.app;
	engineSetting.hInstance = EngineDescription.hInstance;
	engineSetting.resolution = Vector2(static_cast<float>(EngineDescription.width), static_cast<float>(EngineDescription.height));

	CHECK(app, "app is nullptr", false);
	CHECK(engineSetting.hInstance, "hInstance is nullptr", false);

	LOG("Initlaize Process");

	//지역 설정
#ifdef UNICODE
	_wsetlocale(LC_ALL, TEXT("korean"));
	LOG("언어설정 : 한국");
#endif

	//윈도우 초기화
	CHECK(InitWindow(), "윈도우 초기화 실패", false);

	//매니저 초기화
	CHECK(InitManager(), "매니저 초기화 실패", false);

	//엔진 세팅 초기화
	CHECK(InitEngineSetting(), "엔진 세팅 초기화 실패", false);

	LOG("엔진 초기화 성공");
	return true;
}

WPARAM Engine::Run()
{
	LOG("프로그램 실행");

	//앱 초기화
	CHECK(app->Init(), "앱 초기화 실패", false);

	CMD_OBJ->FlushCommandQueue();

	//프로그램 메인 루프
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
	LOG("윈도우 초기화 시작");

	CHECK(EngineRegisterClass(), "윈도우 클래스 등록 실패", false);
	CHECK(InitInstance(SW_SHOWNORMAL), "윈도우 창 생성 실패", false);

	LOG("윈도우 초기화 성공");
	return true;
}

bool Engine::InitManager()
{
	LOG("매니저 초기화 시작");

	CHECK(RenderManager::GetInstance()->Init(), "렌더링 매니저 초기화 실패", false);
	CHECK(CameraManager::GetInstance()->Init(), "카메라 매니저 초기화 실패", false);
	CHECK(GeometryManager::GetInstance()->Init(), "지오메트리 매니저 초기화 실패", false);
	CHECK(InputManager::GetInstance()->Init(), "입력 매니저 초기화 실패", false);
	CHECK(TimerManager::GetInstance()->Init(), "타이머 매니저 초기화 실패", false);
	CHECK(ResourceManager::GetInstance()->Init(), "리소스 매니저 초기화 실패", false);

	LOG("매니저 초기화 성공");
	return true;
}

ATOM Engine::EngineRegisterClass()
{
	LOG("윈도우 클래스 등록");
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
	LOG("윈도우 크기 설정 : %d * %d", width, height);
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
	//4X MSAA(Multi Sample Anti-Aliasing) 품질 수준 확인
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = LITERAL::BACK_BUFFER_FORMAT;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;

	HRESULT hr{};
	
	hr = DEVICE_OBJ->GetDevice()->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels));
	CHECK(SUCCEEDED(hr), "4X MSAA 품질 확인 실패", false);

	engineSetting.quality4XMSAA = msQualityLevels.NumQualityLevels;
	CHECK(engineSetting.quality4XMSAA > 0, "예상치 못한 4X MSAA 품질", false);
	LOG("4X MSAA 품질 : %d", engineSetting.quality4XMSAA);

	return true;
}

void Engine::EngineUpdate()
{
	const float deltaTime{ TimerManager::GetInstance()->Update() };
	InputManager::GetInstance()->Update();
	CameraManager::GetInstance()->Update(deltaTime);
	RenderManager::GetInstance()->Update(deltaTime);

	Update(deltaTime);
	Render();
}

void Engine::Update(float DeltaTime)
{
	app->Update(DeltaTime);

	app->LateUpdate(DeltaTime);
}

void Engine::Render()
{
	RenderManager::GetInstance()->RenderBegin();

	CameraManager::GetInstance()->Render();

	app->Render();

	RenderManager::GetInstance()->RenderEnd();
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