#include "SwapChain.h"
#include "Engine\Engine.h"

bool SwapChain::Init(Microsoft::WRL::ComPtr<ID3D12Device> Device, Microsoft::WRL::ComPtr<IDXGIFactory4> Factory, Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQueue)
{
	LOG("스왑체인 초기화 시작");

	EngineSetting engineSetting = GEngine->GetEngineSetting();
	HRESULT hr{};

	swapChain.Reset();

	DXGI_SWAP_CHAIN_DESC swapCahinDesc{};
	swapCahinDesc.BufferDesc.Width = static_cast<UINT>(engineSetting.resolution.x);	//너비 (해상도 x 값)
	swapCahinDesc.BufferDesc.Height = static_cast<UINT>(engineSetting.resolution.y);	//높이 (해상도 y 값)
	swapCahinDesc.BufferDesc.RefreshRate.Numerator = 60;		//화면 주사율 60Hz 설정 (초당 60번 갱신)
	swapCahinDesc.BufferDesc.RefreshRate.Denominator = 1;	//분모를 1로 설정하여 정확한 60Hz 보장
	swapCahinDesc.BufferDesc.Format = LITERAL::BACK_BUFFER_FORMAT;		//백 버퍼의 픽셀 형식(일반적으로 DXGI_FORMAT_R8G8B8A8_UNORM 사용)
	swapCahinDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	//스캔라인 순서를 지정하지 않음 (*1)
	swapCahinDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					//화면 스케일링 옵션을 지정하지 않음 (기본 값) (*2)
	swapCahinDesc.SampleDesc.Count = engineSetting.state4XMSAA ? 4 : 1;											//MSAA의 샘플 개수를 설정
	swapCahinDesc.SampleDesc.Quality = engineSetting.state4XMSAA ? (engineSetting.quality4XMSAA - 1) : 0;		//MSAA의 품질 설정(0이면 기본 값)
	swapCahinDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		//스왑 체인의 버퍼가 렌더 타겟 출력용으로 사용됨
	swapCahinDesc.BufferCount = LITERAL::SWAP_CHAIN_BUFFER_COUNT;		//백 버퍼 개수
	swapCahinDesc.OutputWindow = engineSetting.hWnd;		//출력할 윈도우 핸들
	swapCahinDesc.Windowed = true;							//창 모드(true) 또는 전체 화면 모드(false)
	swapCahinDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		//DirectX 12에서 가장 권장되는 스왑 방식 (*3)
	swapCahinDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	//전체 화면과 창 모드 전환을 허용

	//스캔라인(Scanline) 렌더링 (*1)
	//디스플레이는 화면을 한 줄(Scanline)씩 순차적으로 그리는 방식을 사용
	//인터레이스(Interlaced) 방식과 프로그레시브(Progressive) 방식이 존재
	// 
	//DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED -> 특정 스캔라인 순서를 지정하지 않고 디스플레이 기본 설정을 따름
	//DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE -> 모든 라인을 순서대로 출력 (보통 LCD/LED 모니터에서 사용)
	//DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST -> 인터레이스 방식(짝수 줄 먼저 그린 후 홀수 줄 그리기)
	//DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST -> 인터레이스 방식(홀수 줄 먼저 그린 후 짝수 줄 그리기)

	//스케일링(Scaling) (*2)
	//렌더링된 프레임(이미지)의 크기가 디스플레이 크기와 다를 때 크기를 조정하는 방식
	// 
	//DXGI_MODE_SCALING_UNSPECIFIED -> 스케일링 옵션을 명시적으로 설정하지 않고 디스플레이 기본값을 따름
	//DXGI_MODE_SCALING_CENTERED -> 해상도가 다르면 이미지를 중앙에 배치하고 남은 부분은 검은색(블랙 바)으로 채움
	//DXGI_MODE_SCALING_STRETCHED -> 해상도가 다르면 이미지를 화면 크기에 맞게 늘려서 출력

	//스왑 이펙트(Swap Effect) (*3)
	//백 버퍼(Back Buffer)와 프론트 버퍼(Front Buffer)를 어떻게 교체할 것인지 결정
	// 
	//DXGI_SWAP_EFFECT_DISCARD -> 사용하지 않는 버퍼는 즉시 삭제됨(구형 DXGI 1.0 방식)
	//DXGI_SWAP_EFFECT_SEQUENTIAL -> 프레임을 순차적으로 보존(오래된 방식, vsync 필요)
	//DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL -> 백 버퍼가 뒤집혀서(flip) 사용됨(DXGI 1.2 이상에서 추천)
	//DXGI_SWAP_EFFECT_FLIP_DISCARD -> 가장 최근의 백 버퍼만 유지하며 이전 버퍼를 버림(DirectX 12에서 최적화됨)
	
	hr = Factory->CreateSwapChain(
		CommandQueue.Get(),		//GPU 명령 실행을 담당하는 커맨드 큐를 연결
		&swapCahinDesc,
		swapChain.GetAddressOf());

	CHECK(SUCCEEDED(hr), "스왑체인 생성 실패", false);

	// 디스크립터 힙
	// 리소스를 GPU가 접근할 수 있도록 저장하는곳
	// 각 디스크립터는 특정 크기의 메모리를 차지
	// 해당 크기를 미리 가져와야 정확한 오프셋으로 계산 가능
	rtvDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	LOG("렌더 타겟 뷰 생성 시작");

	D3D12_DESCRIPTOR_HEAP_DESC renderTargetDesc{};
	renderTargetDesc.NumDescriptors = LITERAL::SWAP_CHAIN_BUFFER_COUNT;	//버퍼 개수만큼 디스크립터 생성
	renderTargetDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;				//RTV 타입
	renderTargetDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;			//CPU 전용 힙이므로 플래그 없음
	renderTargetDesc.NodeMask = 0;										//싱글 GPU 환경이므로 0

	hr = Device->CreateDescriptorHeap(&renderTargetDesc, IID_PPV_ARGS(&rtvHeap));
	CHECK(SUCCEEDED(hr), "렌더 타겟 생성 실패", false);
	LOG("렌더 타겟 생성 성공");

	LOG("스왑체인 초기화 성공");
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

	//새로운 해상도에 맞게 스왑 체인의 백 버퍼 크기를 변경
	hr = swapChain->ResizeBuffers(
		LITERAL::SWAP_CHAIN_BUFFER_COUNT,		//버퍼 개수
		static_cast<UINT>(engineSetting.resolution.x),
		static_cast<UINT>(engineSetting.resolution.y),
		LITERAL::BACK_BUFFER_FORMAT,					//버퍼 포맷
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);		//스왑체인 플래그
	CHECK(SUCCEEDED(hr), "버퍼 리사이즈 실패", false);
	currentBackBufferIdx = 0;

	//새로운 백 버퍼 할당 및 RTV 생성
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle{ rtvHeap->GetCPUDescriptorHandleForHeapStart() };
	for (uint32 i = 0; i < LITERAL::SWAP_CHAIN_BUFFER_COUNT; ++i)
	{
		//새로 리사이즈된 백 버퍼(스왑 체인 버퍼)를 가져옴
		hr = swapChain->GetBuffer(
			i,	//스왑 체인 버퍼 인덱스
			IID_PPV_ARGS(&swapChainBuffers[i]));
		CHECK(SUCCEEDED(hr), "스왑체인 버퍼 가져오기 실패", false);
		//RTV(렌더 타겟 뷰)를 생성하여 백 버퍼를 렌더 타겟으로 사용 가능하게 만듦
		Device->CreateRenderTargetView(
			swapChainBuffers[i].Get(),
			nullptr,		//기본 RTV 설정 사용
			rtvHeapHandle);	//RTV가 저장될 디스크립터 힙의 위치
		//다음 RTV 디스크립터 위치로 이동
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
	//새로운 해상도에 맞게 뷰포트(Viewport) 및 시저(Rectangle)를 업데이트
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
