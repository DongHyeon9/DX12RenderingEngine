#include "HullShaderObject.h"

bool HullShaderObject::Init()
{
    LOG("�� ���̴� ������Ʈ �ʱ�ȭ ����");

    CHECK(CreateShader(TEXT("HS_Default"), E_HULL_SHADER_FLAG::DEFAULT), "HS_Default ���� ����", false);

    LOG("�� ���̴� ������Ʈ �ʱ�ȭ ����");
    return true;
}
