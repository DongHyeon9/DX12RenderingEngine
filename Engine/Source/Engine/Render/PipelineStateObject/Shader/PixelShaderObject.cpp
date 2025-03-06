#include "PixelShaderObject.h"

bool PixelShaderObject::Init()
{
	LOG("픽셀 쉐이더 오브젝트 초기화 시작");

	CHECK(CreateShader(TEXT("PS_Default"), E_PIXEL_SHADER_FLAG::DEFAULT), "PS_Default 생성 실패", false);

	LOG("픽셀 쉐이더 오브젝트 초기화 성공");
	return true;
}
