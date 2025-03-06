#include "ShaderObject.h"

bool ShaderObject::Init()
{
    LOG("���̴� ������Ʈ �ʱ�ȭ ����");

	vertexShaderObj = std::make_unique<VertexShaderObject>();
	hullShaderObj = std::make_unique<HullShaderObject>();
	domainShaderObj = std::make_unique<DomainShaderObject>();
	geometryShaderObj = std::make_unique<GeometryShaderObject>();
	pixelShaderObj = std::make_unique<PixelShaderObject>();

	vertexShaderObj->Init();
	hullShaderObj->Init();
	domainShaderObj->Init();
	geometryShaderObj->Init();
	pixelShaderObj->Init();

    LOG("���̴� ������Ʈ �ʱ�ȭ ����");
    return true;
}
