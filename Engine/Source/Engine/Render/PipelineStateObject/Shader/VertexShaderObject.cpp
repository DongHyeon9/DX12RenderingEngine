#include "VertexShaderObject.h"

bool VertexShaderObject::Init()
{
    LOG("버텍스 쉐이더 오브젝트 초기화 시작");

    CHECK(CreateShader(TEXT("VS_Default"), E_VERTEX_SHADER_FLAG::DEFAULT), "VS_Default 생성 실패", false);

    {
        std::vector<D3D12_INPUT_ELEMENT_DESC> desc
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12 + 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12 + 12 + 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };
        
        SetInputLayout(E_VERTEX_SHADER_FLAG::DEFAULT, desc);
    }

    LOG("버텍스 쉐이더 오브젝트 초기화 성공");
    return true;
}

D3D12_INPUT_LAYOUT_DESC VertexShaderObject::GetInputLayout(E_VERTEX_SHADER_FLAG ShaderFlag) const
{
    size_t index = static_cast<size_t>(ShaderFlag);
    return D3D12_INPUT_LAYOUT_DESC{ inputLayouts[index].data(),static_cast<uint32>(inputLayouts[index].size()) };
}

void VertexShaderObject::SetInputLayout(E_VERTEX_SHADER_FLAG ShaderFlag, std::vector<D3D12_INPUT_ELEMENT_DESC> InputLayout)
{
    inputLayouts[static_cast<uint8>(ShaderFlag)] = std::move(InputLayout);
}
