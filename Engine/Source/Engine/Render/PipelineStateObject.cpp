#include "PipelineStateObject.h"

#include "Manager\RenderManager.h"
#include "Engine\Render\DeviceObject.h"

bool PipelineStateObject::Init()
{
    LOG("���������� ������Ʈ ������Ʈ �ʱ�ȭ ����");

	CHECK(blendStateObject->Init(), "���� ������Ʈ ������Ʈ �ʱ�ȭ ����", false);
	CHECK(depthStencilObject->Init(),"���� ���ٽ� ������Ʈ �ʱ�ȭ ����",false);
	CHECK(rasterizerStateObject->Init(),"������ ������ ������Ʈ ������Ʈ �ʱ�ȭ ����",false);
	CHECK(renderTargetObject->Init(),"���� Ÿ�� ������Ʈ �ʱ�ȭ ����",false);
	CHECK(shaderObject->Init(),"���̴� ������Ʈ �ʱ�ȭ ����",false);
	CHECK(rootSignature->Init(),"��Ʈ �ñ״�ó �ʱ�ȭ ����",false);

	CHECK(CreatePipelineStateObject(), "���������� ������Ʈ ������Ʈ ���� ����", false);

    LOG("���������� ������Ʈ ������Ʈ �ʱ�ȭ ����");
    return true;
}

bool PipelineStateObject::CreatePipelineStateObject()
{
	LOG("���������� ������Ʈ ������Ʈ ���� ����");
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

		hr = DEVICE_OBJ->GetDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipelineStateObjects[static_cast<uint8>(E_RENDERING_FLAG::DEFAULT)]));
		CHECK(SUCCEEDED(hr), "DEFAULT ���������� ������Ʈ ���� ����", false);
		LOG("DEFAULT ���������� ������Ʈ ���� ����");
	}

	LOG("���������� ������Ʈ ������Ʈ ���� ����");
	return true;
}
