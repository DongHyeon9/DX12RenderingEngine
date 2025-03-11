#include "CommandObject.h"

CommandObject::~CommandObject()
{
	CloseHandle(hEventCompletion);
}

bool CommandObject::Init(Microsoft::WRL::ComPtr<ID3D12Device> Device)
{
    LOG("Ŀ�ǵ� ������Ʈ �ʱ�ȭ ����");

	HRESULT hr{};
	hr = Device->CreateFence(
		0,						//�ʱ� �ҽ� ��(GPU�� ó���� �۾��� ������ �ǹ�)
		D3D12_FENCE_FLAG_NONE,	//�⺻ �÷��� ����
		IID_PPV_ARGS(&fence));
	CHECK(SUCCEEDED(hr), "�潺 ���� ����", false);
	LOG("�ҽ� ����");

	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;		//DIRECT Ÿ���� Ŀ�ǵ� ť ���� (�׷��Ƚ�, ��ǻ�� �� �Ϲ����� ��� ����)
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;		//�⺻ �������� �÷��׸� ���� (Ư���� ��� ����)

	hr = Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue));
	CHECK(SUCCEEDED(hr), "Ŀ�ǵ� ť ���� ����", false);
	LOG("Ŀ�ǵ� ť ����");

	hr = Device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,		//DIRECT Ÿ���� ��� ����Ʈ�� �Ҵ��� �� �ִ� ��������� ����
		IID_PPV_ARGS(&commandAllocator));
	CHECK(SUCCEEDED(hr), "Ŀ�ǵ� ��������� ���� ����", false);
	LOG("Ŀ�ǵ� ��������� ����");
	
	hr = Device->CreateCommandList(
		0,									//��Ƽ GPU ȯ�濡�� ����� GPU�� ����(���� GPU������ 0�� ���)
		D3D12_COMMAND_LIST_TYPE_DIRECT,		//DIRECT Ÿ���� ��� ����Ʈ�� ����(�Ϲ����� ������ �� �׷��� ���)
		commandAllocator.Get(),				//Ŀ�ǵ� ��������� ����
		nullptr,							//���������� ���� ��ü(PSO)�� �����ϴ� �κ�(nullptr�� �⺻ ���¿��� ����)
		IID_PPV_ARGS(&commandList));
	CHECK(SUCCEEDED(hr), "Ŀ�ǵ� ����Ʈ ���� ����", false);
	LOG("Ŀ�ǵ� ����Ʈ ����");

	//���� ������ Ŀ�ǵ� ����Ʈ�� �⺻������ Open ����
	//Close()�� ȣ���ؾ� GPU�� ������ �� �ֵ��� �غ�� ���·� �����
	//Close()���� ExecuteCommandLists()�� ����Ͽ� ���డ��
	commandList->Close();

	hEventCompletion = CreateEvent(
		nullptr,	//���� �Ӽ� (�⺻�� ���)
		FALSE,		//���� ���� ��� (�ڵ� ����)
		FALSE,		//�ʱ� ���´� ���ȣ(non-signaled) ����
		nullptr);	//�̺�Ʈ �̸� ����

    LOG("Ŀ�ǵ� ������Ʈ �ʱ�ȭ ����");
    return true;
}

bool CommandObject::ResetCmdList()
{
	//���� �������� Ŀ�ǵ尡 �Ϸ�Ǿ����Ƿ� Ŀ�ǵ� ����Ʈ�� ����
	return SUCCEEDED(commandList->Reset(
		commandAllocator.Get(),
		nullptr));		//���������� ���� ��ü ����(nullptr�� �⺻���� ����)
}

bool CommandObject::FlushCommandQueue()
{
	//GPU���� fenceValue �������� ����� �Ϸ��϶�� ��ȣ(Signal)�� ����
	HRESULT hr = commandQueue->Signal(fence.Get(), ++fenceValue);
	CHECK(SUCCEEDED(hr), "�ҽ� ���� ����", false);

	//GPU�� ���� �Ϸ����� �ʾҴٸ� ���
	if (fence->GetCompletedValue() < fenceValue)
	{
		//GPU�� currentFence ������ �۾��� �Ϸ��ϸ� �̺�Ʈ�� signaled ���·� ����
		hr = fence->SetEventOnCompletion(fenceValue, hEventCompletion);
		CHECK(SUCCEEDED(hr), "����ȭ �̺�Ʈ ���� ����", false);

		//�̺�Ʈ�� signaled ���°� �� ������ CPU�� ���
		WaitForSingleObject(hEventCompletion, INFINITE);
	}
	return true;
}

void CommandObject::ExecuteCommandList()
{
	//Ŀ�ǵ� ����Ʈ�� �����Ͽ� ���� �ʱ�ȭ �� ���� �۾��� GPU���� ����
	commandList->Close();
	std::vector<ID3D12CommandList*> cmdLists
	{
		commandList.Get()
	};
	commandQueue->ExecuteCommandLists(static_cast<UINT>(cmdLists.size()), cmdLists.data());
}
