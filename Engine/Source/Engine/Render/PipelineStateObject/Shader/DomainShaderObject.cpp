#include "DomainShaderObject.h"

bool DomainShaderObject::Init()
{
    LOG("������ ���̴� ������Ʈ �ʱ�ȭ ����");

    CHECK(CreateShader(TEXT("DS_Default"), E_DOMAIN_SHADER_FLAG::DEFAULT), "DS_Default ���� ����", false);

    LOG("������ ���̴� ������Ʈ �ʱ�ȭ ����");
    return true;
}
