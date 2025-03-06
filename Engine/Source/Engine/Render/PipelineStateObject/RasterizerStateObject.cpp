#include "RasterizerStateObject.h"

bool RasterizerStateObject::Init()
{
    LOG("�����Ͷ����� ������Ʈ ������Ʈ �ʱ�ȭ");

    D3D12_RASTERIZER_DESC desc{};
    desc.FillMode = D3D12_FILL_MODE_SOLID;
    desc.CullMode = D3D12_CULL_MODE_BACK;
    desc.FrontCounterClockwise = false;
    desc.DepthClipEnable = true;
    desc.MultisampleEnable = false;
    SetRasterizerState(E_RASTERIZER_STATE_FLAG::SOLID, desc);

    desc.FrontCounterClockwise = true;
    SetRasterizerState(E_RASTERIZER_STATE_FLAG::SOLID_CCW, desc);

    desc.FillMode = D3D12_FILL_MODE_WIREFRAME;
    SetRasterizerState(E_RASTERIZER_STATE_FLAG::WIRE_FRAME_CCW, desc);

    desc.FrontCounterClockwise = false;
    SetRasterizerState(E_RASTERIZER_STATE_FLAG::WIRE_FRAME, desc);

    //TODO
    //����Ʈ ���μ��� �����
    SetRasterizerState(E_RASTERIZER_STATE_FLAG::POST_PROCESSING, desc);

    LOG("�����Ͷ����� ������Ʈ ������Ʈ �ʱ�ȭ ����");
    return true;
}

void RasterizerStateObject::SetRasterizerState(E_RASTERIZER_STATE_FLAG RasterizeStateFlag, D3D12_RASTERIZER_DESC Desc)
{
    rasterizeStates[static_cast<uint8>(RasterizeStateFlag)] = std::move(Desc);
}
