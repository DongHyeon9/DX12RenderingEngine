#include "GeometryShaderObject.h"

bool GeometryShaderObject::Init()
{
    LOG("������Ʈ�� ���̴� ������Ʈ �ʱ�ȭ ����");

    CHECK(CreateShader(TEXT("GS_Default"), E_GEOMETRY_SHADER_FLAG::DEFAULT), "GS_Default ���� ����", false);

    LOG("������Ʈ�� ���̴� ������Ʈ �ʱ�ȭ ����");
    return true;
}
