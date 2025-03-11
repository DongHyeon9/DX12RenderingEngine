#include "ShaderObject.h"

bool ShaderObject::Init()
{
    LOG("���̴� ������Ʈ �ʱ�ȭ ����");

	vertexShaderObj = std::make_unique<VertexShaderObject>();
	hullShaderObj = std::make_unique<HullShaderObject>();
	domainShaderObj = std::make_unique<DomainShaderObject>();
	geometryShaderObj = std::make_unique<GeometryShaderObject>();
	pixelShaderObj = std::make_unique<PixelShaderObject>();

	CHECK(vertexShaderObj->Init(), "���ؽ� ���̴� �ʱ�ȭ ����", false);
	CHECK(hullShaderObj->Init(), "�� ���̴� �ʱ�ȭ ����", false);
	CHECK(domainShaderObj->Init(), "������ ���̴� �ʱ�ȭ ����", false);
	CHECK(geometryShaderObj->Init(), "������Ʈ�� ���̴� �ʱ�ȭ ����", false);
	CHECK(pixelShaderObj->Init(), "�ȼ� ���̴� �ʱ�ȭ ����", false);

    LOG("���̴� ������Ʈ �ʱ�ȭ ����");
    return true;
}
