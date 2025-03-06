#pragma once
#include "Header\EngineCoreMinimal.h"

class CommandObject
{
public:

protected:

private:
	//GPU에게 내릴 명령을 저장하는 큐
	//GPU가 명령을 처리하는 우선순위를 조정
	//여러 개의 명령 리스트를 실행할 수 있도록 함
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue{};
	//커맨드 리스트가 사용할 메모리를 관리하는 객체
	//커맨드 리스트가 사용하는 메모리를 제공
	//실제 실행을 담당하진 않음
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator{};
	//GPU가 실행할 명령을 기록
	//실행을 위해 커맨드 큐에 제출됨
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList{};

	//GPU와 CPU간의 동기화를 위한 객체
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

