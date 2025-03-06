#include "HullShaderObject.h"

bool HullShaderObject::Init()
{
    LOG("헐 쉐이더 오브젝트 초기화 시작");

    CHECK(CreateShader(TEXT("HS_Default"), E_HULL_SHADER_FLAG::DEFAULT), "HS_Default 생성 실패", false);

    LOG("헐 쉐이더 오브젝트 초기화 성공");
    return true;
}
