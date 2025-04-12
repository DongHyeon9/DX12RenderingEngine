#pragma once
#include "Header\EngineCore.h"

class DeviceObject;
class CommandObject;
class SwapChainObject;
class PipelineStateObject;

enum class E_RENDERING_FLAG : uint8;

class RenderManager
{
	DECLARE_SINGLETON(RenderManager);
public:

protected:

private:
	std::shared_ptr<DeviceObject> device{};
	std::shared_ptr<CommandObject> commandObject{};
	std::shared_ptr<SwapChainObject> swapChain{};
	std::shared_ptr<PipelineStateObject> pipelineStateObject{};

public:
	bool Init();

	void Update(float DeltaTime);
	void RenderBegin();
	void RenderEnd();
	void SetPipelineState(E_RENDERING_FLAG Flag);

	FORCEINLINE std::shared_ptr<DeviceObject> GetDeviceObject()const { return device; }
	FORCEINLINE std::shared_ptr<CommandObject> GetCommandObject()const { return commandObject; }
	FORCEINLINE std::shared_ptr<SwapChainObject> GetSwapChainObject()const { return swapChain; }
	FORCEINLINE std::shared_ptr<PipelineStateObject> GetPipelineStateObject()const { return pipelineStateObject; }

protected:

private:
	//πÈ πˆ∆€(Render Target View, RTV)øÕ ±Ì¿Ã/Ω∫≈ŸΩ« ∫‰(Depth Stencil View, DSV)∏¶ ¿Áº≥¡§
	bool ResizeWindow();
};

