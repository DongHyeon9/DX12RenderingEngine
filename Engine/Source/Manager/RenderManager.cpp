#include "RenderManager.h"

#include "Engine\Render\DeviceObject.h"
#include "Engine\Render\CommandObject.h"
#include "Engine\Render\PipelineStateObject.h"
#include "Engine\Render\SwapChainObject.h"

bool RenderManager::Init()
{
	LOG("렌더 매니저 초기화 시작");

	device = std::make_shared<DeviceObject>();
	commandObject = std::make_shared<CommandObject>();
	swapChain = std::make_shared<SwapChainObject>();
	pipelineStateObject = std::make_shared<PipelineStateObject>();

	CHECK(device->Init(), "디바이스 초기화 실패", false);
	CHECK(commandObject->Init(), "커맨드 오브젝트 초기화 실패", false);
	CHECK(swapChain->Init(), "스왑체인 초기화 실패", false);
	CHECK(pipelineStateObject->Init(), "파이프라인 스테이트 초기화 실패", false);

	CHECK(ResizeWindow(), "윈도우 사이즈 조정 실패", false);

	LOG("렌더 매니저 초기화 성공");
    return true;
}

void RenderManager::Update(float DeltaTime)
{
}

void RenderManager::RenderBegin()
{
	HRESULT hr{};

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList = commandObject->GetCommandList();
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAlloc = commandObject->GetCommandAllocator();

	commandObject->ResetCmdList();

	//전면버퍼와 후면버퍼의 교체
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		swapChain->GetBackBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	pipelineStateObject->GetRootSignature()->Clear();

	cmdList->SetGraphicsRootSignature(pipelineStateObject->GetRootSignature()->GetSignature().Get());

	ID3D12DescriptorHeap* tableDescriptorHeap = pipelineStateObject->GetRootSignature()->GetDescriptorTable().Get();
	cmdList->SetDescriptorHeaps(1, &tableDescriptorHeap);

	cmdList->ResourceBarrier(1, &barrier);

	cmdList->RSSetViewports(1, &swapChain->GetViewport());
	cmdList->RSSetScissorRects(1, &swapChain->GetScissorRect());

	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = swapChain->GetBackBufferRTV();
	cmdList->ClearRenderTargetView(backBufferView, GLOBAL::CLEAR_COLOR, 0, nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = pipelineStateObject->GetDepthStencilObject()->GetDSVHandle();
	cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, &depthStencilView);
	cmdList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0, 0, 0, nullptr);
}

void RenderManager::RenderEnd()
{
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList = commandObject->GetCommandList();

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		swapChain->GetBackBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	cmdList->ResourceBarrier(1, &barrier);

	commandObject->ExecuteCommandList();

	swapChain->Present();

	commandObject->FlushCommandQueue();

	swapChain->SwapIndex();
}

void RenderManager::SetPipelineState(E_RENDERING_FLAG Flag)
{
	commandObject->GetCommandList()->SetPipelineState(pipelineStateObject->GetPipelineStateObject(Flag).Get());
}

bool RenderManager::ResizeWindow()
{
	LOG("스왑체인 리사이즈 시작");

	//이전 커맨드가 모두 끝날 때까지 대기
	CHECK(commandObject->FlushCommandQueue(), "커맨드 큐 비우기 실패", false);

	HRESULT hr{};

	CHECK(commandObject->ResetCmdList(), "커맨드 리스트 리셋 실패", false);

	CHECK(SUCCEEDED(hr), "커맨드 리스트 리셋 실패", false);

	//기존의 스왑 체인 버퍼와 뎁스 스텐실 버퍼를 해제
	swapChain->ResetSwapChain();
	pipelineStateObject->GetDepthStencilObject()->ResetDepthStencilBuffer();

	CHECK(swapChain->CreateSwapChainBuffer(), "스왑체인 버퍼 생성 실패", false);
	CHECK(pipelineStateObject->GetDepthStencilObject()->CreateDepthStencilBuffer(), "뎁스 스텐실 버퍼 생성 실패", false);

	//뎁스 스텐실 버퍼를 D3D12_RESOURCE_STATE_DEPTH_WRITE 상태로 변경하여 렌더링 시 깊이 쓰기 가능하도록 설정
	D3D12_RESOURCE_BARRIER barrier{ CD3DX12_RESOURCE_BARRIER::Transition(
		pipelineStateObject->GetDepthStencilObject()->GetDepthStencilBuffer().Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_DEPTH_WRITE) };
	commandObject->GetCommandList()->ResourceBarrier(1, &barrier);
	commandObject->ExecuteCommandList();

	CHECK(commandObject->FlushCommandQueue(), "커맨드 큐 비우기 실패", false);

	swapChain->SetScreenViewport();
	swapChain->SetScissorRect();

	LOG("스왑체인 리사이즈 성공");
	return true;
}
