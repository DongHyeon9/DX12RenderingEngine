#include "PixelShaderObject.h"

bool PixelShaderObject::Init()
{
	LOG("�ȼ� ���̴� ������Ʈ �ʱ�ȭ ����");

	CHECK(CreateShader(TEXT("PS_Default"), E_PIXEL_SHADER_FLAG::DEFAULT), "PS_Default ���� ����", false);

	LOG("�ȼ� ���̴� ������Ʈ �ʱ�ȭ ����");
	return true;
}
