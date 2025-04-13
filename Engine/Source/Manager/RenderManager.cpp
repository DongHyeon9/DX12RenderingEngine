#include "RenderManager.h"

#include "Engine\Render\DeviceObject.h"
#include "Engine\Render\CommandObject.h"
#include "Engine\Render\PipelineStateObject.h"
#include "Engine\Render\SwapChainObject.h"

bool RenderManager::Init()
{
	LOG("���� �Ŵ��� �ʱ�ȭ ����");

	device = std::make_shared<DeviceObject>();
	commandObject = std::make_shared<CommandObject>();
	swapChain = std::make_shared<SwapChainObject>();
	pipelineStateObject = std::make_shared<PipelineStateObject>();

	CHECK(device->Init(), "����̽� �ʱ�ȭ ����", false);
	CHECK(commandObject->Init(), "Ŀ�ǵ� ������Ʈ �ʱ�ȭ ����", false);
	CHECK(swapChain->Init(), "����ü�� �ʱ�ȭ ����", false);
	CHECK(pipelineStateObject->Init(), "���������� ������Ʈ �ʱ�ȭ ����", false);

	CHECK(ResizeWindow(), "������ ������ ���� ����", false);

	LOG("���� �Ŵ��� �ʱ�ȭ ����");
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

	//������ۿ� �ĸ������ ��ü
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
	LOG("����ü�� �������� ����");

	//���� Ŀ�ǵ尡 ��� ���� ������ ���
	CHECK(commandObject->FlushCommandQueue(), "Ŀ�ǵ� ť ���� ����", false);

	HRESULT hr{};

	CHECK(commandObject->ResetCmdList(), "Ŀ�ǵ� ����Ʈ ���� ����", false);

	CHECK(SUCCEEDED(hr), "Ŀ�ǵ� ����Ʈ ���� ����", false);

	//������ ���� ü�� ���ۿ� ���� ���ٽ� ���۸� ����
	swapChain->ResetSwapChain();
	pipelineStateObject->GetDepthStencilObject()->ResetDepthStencilBuffer();

	CHECK(swapChain->CreateSwapChainBuffer(), "����ü�� ���� ���� ����", false);
	CHECK(pipelineStateObject->GetDepthStencilObject()->CreateDepthStencilBuffer(), "���� ���ٽ� ���� ���� ����", false);

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
