#pragma once
#include "Header\EngineCoreMinimal.h"

class CommandObject
{
public:

protected:

private:
	//GPU���� ���� ����� �����ϴ� ť
	//GPU�� ����� ó���ϴ� �켱������ ����
	//���� ���� ��� ����Ʈ�� ������ �� �ֵ��� ��
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue{};
	//Ŀ�ǵ� ����Ʈ�� ����� �޸𸮸� �����ϴ� ��ü
	//Ŀ�ǵ� ����Ʈ�� ����ϴ� �޸𸮸� ����
	//���� ������ ������� ����
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator{};
	//GPU�� ������ ����� ���
	//������ ���� Ŀ�ǵ� ť�� �����
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList{};

	//GPU�� CPU���� ����ȭ�� ���� ��ü
	Microsoft::WRL::ComPtr<ID3D12Fence> fence{};
	uint64 fenceValue{};
	HANDLE hEventCompletion{};

public:
	~CommandObject();
	bool Init(Microsoft::WRL::ComPtr<ID3D12Device> Device);
	bool ResetCmdList();
	bool FlushCommandQueue();
	void ExecuteCommandList();

	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue()const { return commandQueue; }
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetCommandAllocator()const { return commandAllocator; }
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList()const { return commandList; }

protected:

private:

};

