#pragma once
#include "PipelineStateObject\BlendStateObject.h"
#include "PipelineStateObject\DepthStencilObject.h"
#include "PipelineStateObject\RasterizerStateObject.h"
#include "PipelineStateObject\RenderTargetObject.h"
#include "PipelineStateObject\ShaderObject.h"
#include "PipelineStateObject\RootSignature.h"

enum class E_RENDERING_FLAG
{
	DEFAULT = 0,

	END
};

class PipelineStateObject
{
public:

protected:

private:
	std::shared_ptr<BlendStateObject> blendStateObject{ std::make_shared<BlendStateObject>() };
	std::shared_ptr<DepthStencilObject> depthStencilObject{ std::make_shared<DepthStencilObject>() };
	std::shared_ptr<RasterizerStateObject> rasterizerStateObject{ std::make_shared<RasterizerStateObject>() };
	std::shared_ptr<RenderTargetObject> renderTargetObject{ std::make_shared<RenderTargetObject>() };
	std::shared_ptr<ShaderObject> shaderObject{ std::make_shared<ShaderObject>() };
	std::shared_ptr<RootSignature> rootSignature{ std::make_shared<RootSignature>() };

	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, static_cast<uint8>(E_RENDERING_FLAG::END)> pipelineStateObjects{};

public:
	bool Init(Microsoft::WRL::ComPtr<ID3D12Device> Device);

	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineStateObject(E_RENDERING_FLAG Flag) { return pipelineStateObjects[static_cast<uint8>(Flag)]; }
	FORCEINLINE std::shared_ptr<BlendStateObject> GetBlendStateObject()const { return blendStateObject; }
	FORCEINLINE std::shared_ptr<DepthStencilObject> GetDepthStencilObject()const { return depthStencilObject; }
	FORCEINLINE std::shared_ptr<RasterizerStateObject> GetRasterizerStateObject()const { return rasterizerStateObject; }
	FORCEINLINE std::shared_ptr<RenderTargetObject> GetRenderTargetObject()const { return renderTargetObject; }
	FORCEINLINE std::shared_ptr<ShaderObject> GetShaderObject()const { return shaderObject; }
	FORCEINLINE std::shared_ptr<RootSignature> GetRootSignature()const { return rootSignature; }

protected:
	bool CreatePipelineStateObject(Microsoft::WRL::ComPtr<ID3D12Device> Device);

private:

};

