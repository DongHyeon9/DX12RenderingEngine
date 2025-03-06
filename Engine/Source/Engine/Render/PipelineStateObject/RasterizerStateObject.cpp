#include "RasterizerStateObject.h"

bool RasterizerStateObject::Init()
{
    LOG("레스터라이즈 스테이트 오브젝트 초기화");

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
    //포스트 프로세싱 만들기
    SetRasterizerState(E_RASTERIZER_STATE_FLAG::POST_PROCESSING, desc);

    LOG("레스터라이즈 스테이트 오브젝트 초기화 성공");
    return true;
}

void RasterizerStateObject::SetRasterizerState(E_RASTERIZER_STATE_FLAG RasterizeStateFlag, D3D12_RASTERIZER_DESC Desc)
{
    rasterizeStates[static_cast<uint8>(RasterizeStateFlag)] = std::move(Desc);
}
