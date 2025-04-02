#pragma once
#include "Header\EngineCore.h"
#include "Render\Device.h"
#include "Render\CommandObject.h"
#include "Render\PipelineStateObject.h"
#include "Render\SwapChain.h"

class AppBase;

class Engine
{
public:

protected:

private:
	std::shared_ptr<AppBase> app{};
	EngineSetting engineSetting{};

	Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer{};

	std::unique_ptr<Device> device{};
	std::unique_ptr<CommandObject> commandObject{};
	std::unique_ptr<SwapChain> swapChain{};
	std::unique_ptr<PipelineStateObject> pipelineStateObject{};

public:
	bool Init(const EngineDesc& EngineDescription);
	WPARAM Run();

	FORCEINLINE std::shared_ptr<AppBase> GetApp()const { return app; }
	FORCEINLINE const EngineSetting& GetEngineSetting()const { return engineSetting; }

	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12Device> GetDevice()const { return device->GetDevice(); }
	FORCEINLINE Microsoft::WRL::ComPtr<IDXGIFactory4> GetFactory()const { return device->GetFactory(); }

	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue()const { return commandObject->GetCommandQueue(); }
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetCommandAllocator()const { return commandObject->GetCommandAllocator(); }
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList()const { return commandObject->GetCommandList(); }

	FORCEINLINE Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain()const { return swapChain->GetSwapChain(); }

protected:

private:
	bool InitWindow();
	bool InitDirectX();
	bool InitManager();

	ATOM EngineRegisterClass();
	BOOL InitInstance(int CmdShow);
	bool InitEngineSetting();

	void EngineUpdate();
	void Update(float DeltaTime);

	void RenderBegin();
	void Render();
	void RenderEnd();

	//πÈ πˆ∆€(Render Target View, RTV)øÕ ±Ì¿Ã/Ω∫≈ŸΩ« ∫‰(Depth Stencil View, DSV)∏¶ ¿Áº≥¡§
	bool ResizeWindow();
};

static LRESULT CALLBACK WndProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);