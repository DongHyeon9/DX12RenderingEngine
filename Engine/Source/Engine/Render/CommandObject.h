#pragma once
#include "Header\EngineCore.h"

class CommandObject
{
public:

protected:

private:
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue{};
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator{};
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList{};
	Microsoft::WRL::ComPtr<ID3D12Fence> fence{};
	uint64 fenceValue{};
	HANDLE hEventCompletion{};

public:
	~CommandObject();
	bool Init();
	bool ResetCmdList();
	bool FlushCommandQueue();
	void ExecuteCommandList();

	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue()const { return commandQueue; }
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetCommandAllocator()const { return commandAllocator; }
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList()const { return commandList; }

protected:

private:

};

