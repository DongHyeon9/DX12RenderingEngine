#include "BlendStateObject.h"

bool BlendStateObject::Init()
{
    LOG("블렌드 스테이트 오브젝트 초기화");

    D3D12_BLEND_DESC desc{};
    D3D12_RENDER_TARGET_BLEND_DESC renderTargetDesc{};
    desc.AlphaToCoverageEnable = false;
    desc.IndependentBlendEnable = false;
    renderTargetDesc.SrcBlend = D3D12_BLEND_ONE;
    renderTargetDesc.DestBlend = D3D12_BLEND_ONE;
    renderTargetDesc.BlendOp = D3D12_BLEND_OP_ADD;
    renderTargetDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
    renderTargetDesc.DestBlendAlpha = D3D12_BLEND_ONE;
    renderTargetDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    renderTargetDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    for (auto& target : desc.RenderTarget)
    {
        target = renderTargetDesc;
    }
    SetBlendState(E_BLEND_STATE_FLAG::DEFAULT, desc);

    desc.AlphaToCoverageEnable = true;
    renderTargetDesc.BlendEnable = true;
    renderTargetDesc.SrcBlend = D3D12_BLEND_BLEND_FACTOR;
    renderTargetDesc.DestBlend = D3D12_BLEND_INV_BLEND_FACTOR;
    renderTargetDesc.DestBlendAlpha = D3D12_BLEND_ONE;
    for (auto& target : desc.RenderTarget)
    {
        target = renderTargetDesc;
    }
    SetBlendState(E_BLEND_STATE_FLAG::MIRROR, desc);

    LOG("블렌드 스테이트 오브젝트 초기화 성공");
    return true;
}

void BlendStateObject::SetBlendState(E_BLEND_STATE_FLAG BlendStateFlag, D3D12_BLEND_DESC Desc)
{
    blendStates[static_cast<uint8>(BlendStateFlag)] = std::move(Desc);
}
