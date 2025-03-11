#include "ShaderObject.h"

bool ShaderObject::Init()
{
    LOG("쉐이더 오브젝트 초기화 시작");

	vertexShaderObj = std::make_unique<VertexShaderObject>();
	hullShaderObj = std::make_unique<HullShaderObject>();
	domainShaderObj = std::make_unique<DomainShaderObject>();
	geometryShaderObj = std::make_unique<GeometryShaderObject>();
	pixelShaderObj = std::make_unique<PixelShaderObject>();

	CHECK(vertexShaderObj->Init(), "버텍스 쉐이더 초기화 실패", false);
	CHECK(hullShaderObj->Init(), "헐 쉐이더 초기화 실패", false);
	CHECK(domainShaderObj->Init(), "도메인 쉐이더 초기화 실패", false);
	CHECK(geometryShaderObj->Init(), "지오메트리 쉐이더 초기화 실패", false);
	CHECK(pixelShaderObj->Init(), "픽셀 쉐이더 초기화 실패", false);

    LOG("쉐이더 오브젝트 초기화 성공");
    return true;
}
