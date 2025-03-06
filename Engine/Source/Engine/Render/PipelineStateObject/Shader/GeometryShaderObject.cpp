#include "GeometryShaderObject.h"

bool GeometryShaderObject::Init()
{
    LOG("지오메트리 쉐이더 오브젝트 초기화 시작");

    CHECK(CreateShader(TEXT("GS_Default"), E_GEOMETRY_SHADER_FLAG::DEFAULT), "GS_Default 생성 실패", false);

    LOG("지오메트리 쉐이더 오브젝트 초기화 성공");
    return true;
}
