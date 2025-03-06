#include "ShaderObject.h"

bool ShaderObject::Init()
{
    LOG("쉐이더 오브젝트 초기화 시작");

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

    LOG("쉐이더 오브젝트 초기화 성공");
    return true;
}
