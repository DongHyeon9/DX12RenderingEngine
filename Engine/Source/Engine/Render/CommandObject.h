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

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> resCmdAlloc{};
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> resCmdList{};

	Microsoft::WRL::ComPtr<ID3D12Fence> fence{};
	uint64 fenceValue{};
	HANDLE hEventCompletion{};

public:
	~CommandObject();
	bool Init();
	bool ResetCmdList();
	bool FlushCommandQueue();
	void ExecuteCommandList();
	void ExecuteResourceCommandList();

	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue()const { return commandQueue; }
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetCommandAllocator()const { return commandAllocator; }
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList()const { return commandList; }

	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetResourceCommandList()const { return resCmdList; }
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetResourceCommandAllocator()const { return resCmdAlloc; }

protected:

private:

};

