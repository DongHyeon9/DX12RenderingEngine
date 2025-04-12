#pragma once
#include "PipelineStateObject\BlendStateObject.h"
#include "PipelineStateObject\DepthStencilObject.h"
#include "PipelineStateObject\RasterizerStateObject.h"
#include "PipelineStateObject\RenderTargetObject.h"
#include "PipelineStateObject\ShaderObject.h"
#include "PipelineStateObject\RootSignatureObject.h"

enum class E_RENDERING_FLAG : uint8
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
	std::shared_ptr<RootSignatureObject> rootSignature{ std::make_shared<RootSignatureObject>() };

	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, static_cast<uint8>(E_RENDERING_FLAG::END)> pipelineStateObjects{};

public:
	bool Init();


	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineStateObject(E_RENDERING_FLAG Flag) { return pipelineStateObjects[static_cast<uint8>(Flag)]; }
	FORCEINLINE std::shared_ptr<BlendStateObject> GetBlendStateObject()const { return blendStateObject; }
	FORCEINLINE std::shared_ptr<DepthStencilObject> GetDepthStencilObject()const { return depthStencilObject; }
	FORCEINLINE std::shared_ptr<RasterizerStateObject> GetRasterizerStateObject()const { return rasterizerStateObject; }
	FORCEINLINE std::shared_ptr<RenderTargetObject> GetRenderTargetObject()const { return renderTargetObject; }
	FORCEINLINE std::shared_ptr<ShaderObject> GetShaderObject()const { return shaderObject; }
	FORCEINLINE std::shared_ptr<RootSignatureObject> GetRootSignature()const { return rootSignature; }

protected:
	bool CreatePipelineStateObject();

private:

};

