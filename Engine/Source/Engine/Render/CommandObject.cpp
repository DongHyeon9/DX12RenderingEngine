#include "CommandObject.h"

#include "Manager\RenderManager.h"
#include "Engine\Render\DeviceObject.h"

CommandObject::~CommandObject()
{
	CloseHandle(hEventCompletion);
}

bool CommandObject::Init()
{
    LOG("커맨드 오브젝트 초기화 시작");

	HRESULT hr{};
	Microsoft::WRL::ComPtr<ID3D12Device> device = DEVICE_OBJ->GetDevice();
	hr = device->CreateFence(
		0,						//초기 팬스 값(GPU가 처리할 작업이 없음을 의미)
		D3D12_FENCE_FLAG_NONE,	//기본 플래그 설정
		IID_PPV_ARGS(&fence));
	CHECK(SUCCEEDED(hr), "펜스 생성 실패", false);
	LOG("팬스 생성");

	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;		//DIRECT 타입의 커맨드 큐 생성 (그래픽스, 컴퓨팅 등 일반적인 명령 실행)
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;		//기본 설정으로 플래그를 설정 (특별한 기능 없음)

	hr = device->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue));
	CHECK(SUCCEEDED(hr), "커맨드 큐 생성 실패", false);
	LOG("커맨드 큐 생성");

	hr = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,		//DIRECT 타입의 명령 리스트를 할당할 수 있는 얼로케이터 생성
		IID_PPV_ARGS(&commandAllocator));
	CHECK(SUCCEEDED(hr), "커맨드 얼로케이터 생성 실패", false);
	LOG("커맨드 얼로케이터 생성");
	
	hr = device->CreateCommandList(
		0,									//멀티 GPU 환경에서 사용할 GPU를 지정(단일 GPU에서는 0을 사용)
		D3D12_COMMAND_LIST_TYPE_DIRECT,		//DIRECT 타입의 명령 리스트를 생성(일반적인 렌더링 및 그래픽 명령)
		commandAllocator.Get(),				//커맨드 얼로케이터 지정
		nullptr,							//파이프라인 상태 객체(PSO)를 지정하는 부분(nullptr은 기본 상태에서 시작)
		IID_PPV_ARGS(&commandList));
	CHECK(SUCCEEDED(hr), "커맨드 리스트 생성 실패", false);
	LOG("커맨드 리스트 생성");

	//새로 생성된 커맨드 리스트는 기본적으로 Open 상태
	//Close()를 호출해야 GPU가 실행할 수 있도록 준비된 상태로 변경됨
	//Close()이후 ExecuteCommandLists()를 사용하여 실행가능
	commandList->Close();

	hEventCompletion = CreateEvent(
		nullptr,	//보안 속성 (기본값 사용)
		FALSE,		//수동 리셋 모드 (자동 리셋)
		FALSE,		//초기 상태는 비신호(non-signaled) 상태
		nullptr);	//이벤트 이름 없음

    LOG("커맨드 오브젝트 초기화 성공");
    return true;
}

bool CommandObject::ResetCmdList()
{
	//이전 프레임의 커맨드가 완료되었으므로 커맨드 리스트를 리셋
	return SUCCEEDED(commandList->Reset(
		commandAllocator.Get(),
		nullptr));		//파이프라인 상태 개체 지정(nullptr은 기본상태 유지)
}

bool CommandObject::FlushCommandQueue()
{
	//GPU에게 fenceValue 값까지의 명령을 완료하라고 신호(Signal)를 보냄
	HRESULT hr = commandQueue->Signal(fence.Get(), ++fenceValue);
	CHECK(SUCCEEDED(hr), "팬스 설정 실패", false);

	//GPU가 아직 완료하지 않았다면 대기
	if (fence->GetCompletedValue() < fenceValue)
	{
		//GPU가 currentFence 값까지 작업을 완료하면 이벤트를 signaled 상태로 변경
		hr = fence->SetEventOnCompletion(fenceValue, hEventCompletion);
		CHECK(SUCCEEDED(hr), "동기화 이벤트 설정 실패", false);

		//이벤트가 signaled 상태가 될 때까지 CPU가 대기
		WaitForSingleObject(hEventCompletion, INFINITE);
	}
	return true;
}

void CommandObject::ExecuteCommandList()
{
	//커맨드 리스트를 실행하여 버퍼 초기화 및 설정 작업을 GPU에서 수행
	commandList->Close();
	std::vector<ID3D12CommandList*> cmdLists
	{
		commandList.Get()
	};
	commandQueue->ExecuteCommandLists(static_cast<UINT>(cmdLists.size()), cmdLists.data());
}
