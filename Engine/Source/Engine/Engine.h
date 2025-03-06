#pragma once
#include "Header\EngineCoreMinimal.h"

class AppBase;
class Device;
class CommandObject;
class SwapChain;
class PipelineStateObject;

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

extern std::unique_ptr<Engine> GEngine;
static LRESULT CALLBACK WndProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);