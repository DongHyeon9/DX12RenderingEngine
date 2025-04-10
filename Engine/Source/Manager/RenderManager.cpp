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
