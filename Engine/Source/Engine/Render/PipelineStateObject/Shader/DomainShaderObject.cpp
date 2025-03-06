#include "DomainShaderObject.h"

bool DomainShaderObject::Init()
{
    LOG("도메인 쉐이더 오브젝트 초기화 시작");

    CHECK(CreateShader(TEXT("DS_Default"), E_DOMAIN_SHADER_FLAG::DEFAULT), "DS_Default 생성 실패", false);

    LOG("도메인 쉐이더 오브젝트 초기화 성공");
    return true;
}
