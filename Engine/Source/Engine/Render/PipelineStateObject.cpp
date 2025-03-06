#include "PipelineStateObject.h"

bool PipelineStateObject::Init(Microsoft::WRL::ComPtr<ID3D12Device> Device)
{
    LOG("파이프라인 스테이트 오브젝트 초기화 시작");

	blendStateObject->Init();
	depthStencilObject->Init(Device);
	rasterizerStateObject->Init();
	renderTargetObject->Init();
	shaderObject->Init();
	rootSignature->Init(Device);

	CHECK(CreatePipelineStateObject(Device), "파이프라인 스테이트 오브젝트 생성 실패", false);

    LOG("파이프라인 스테이트 오브젝트 초기화 성공");
    return true;
}

bool PipelineStateObject::CreatePipelineStateObject(Microsoft::WRL::ComPtr<ID3D12Device> Device)
{
	LOG("파이프라인 스테이트 오브젝트 생성 시작");
	HRESULT hr{};
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
		desc.pRootSignature = rootSignature->GetRootSignature(E_ROOT_SIGNATURE_FLAG::DEFAULT).Get();
		desc.SampleDesc.Count = 1;
		desc.SampleMask = UINT_MAX;

		desc.InputLayout = shaderObject->GetInputLayoutDesc(E_VERTEX_SHADER_FLAG::DEFAULT);
		desc.VS = shaderObject->GetVertexShader(E_VERTEX_SHADER_FLAG::DEFAULT);
		desc.PS = shaderObject->GetPixelShader(E_PIXEL_SHADER_FLAG::DEFAULT);
		desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		desc.NumRenderTargets = 1;
		desc.RTVFormats[0] = LITERAL::BACK_BUFFER_FORMAT;

		desc.RasterizerState = rasterizerStateObject->GetRasterizeState(E_RASTERIZER_STATE_FLAG::SOLID);

		desc.DepthStencilState = depthStencilObject->GetDepthStencilState(E_DEPTH_STENCIL_STATE_FLAG::DEFAULT);
		desc.DSVFormat = LITERAL::DEPTH_STENCIL_FORMAT;

		hr = Device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipelineStateObjects[static_cast<uint8>(E_RENDERING_FLAG::DEFAULT)]));
		CHECK(SUCCEEDED(hr), "DEFAULT 파이프라인 스테이트 생성 실패", false);
		LOG("DEFAULT 파이프라인 스테이트 생성 성공");
	}

	LOG("파이프라인 스테이트 오브젝트 생성 성공");
	return true;
}
