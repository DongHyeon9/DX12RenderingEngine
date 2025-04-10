#include "RootSignatureObject.h"

#include "Manager\RenderManager.h"
#include "Engine\Render\DeviceObject.h"

#include "Resource\Internal\DescriptorTable.h"
#include "Resource\Internal\ConstantBuffer.h"

bool RootSignatureObject::Init()
{
    LOG("��Ʈ �ñ״�ó �ʱ�ȭ ����");

    CHECK(!CreateSamplerDesc(), "���÷� ��ũ���� ���� ����", false);
    CHECK(!CreateRootSignature(), "��Ʈ �ñ״�ó ���� ����", false);

    descriptorTable = std::make_shared<DescriptorTable>();
    descriptorTable->Init(256);

    CHECK(!CreateConstantBuffer(E_CBV_REGISTER::B0, E_CONSTANT_BUFFER_TYPE::CAMERA, sizeof(CameraData), 1), "ī�޶� �ܽ�źƮ ���� ���� ����", false);
    CHECK(!CreateConstantBuffer(E_CBV_REGISTER::B1, E_CONSTANT_BUFFER_TYPE::LIGHT, sizeof(LightData), 1), "����Ʈ �ܽ�źƮ ���� ���� ����", false);
    CHECK(!CreateConstantBuffer(E_CBV_REGISTER::B2, E_CONSTANT_BUFFER_TYPE::NORMAL, sizeof(NormalVectorData), 1), "�븻���� �ܽ�źƮ ���� ���� ����", false);

    CHECK(!CreateConstantBuffer(E_CBV_REGISTER::B3, E_CONSTANT_BUFFER_TYPE::TRANSFORM, sizeof(TransformMatrix), 256), "Ʈ������ �ܽ�źƮ ���� ���� ����", false);
    CHECK(!CreateConstantBuffer(E_CBV_REGISTER::B4, E_CONSTANT_BUFFER_TYPE::MATERIAL, sizeof(MaterialParam), 256), "��Ƽ���� �ܽ�źƮ ���� ���� ����", false);

    LOG("��Ʈ �ñ״�ó �ʱ�ȭ ����");
    return true;
}

bool RootSignatureObject::CreateSamplerDesc()
{
    LOG("���÷� ��ũ���� ���� ����");

    D3D12_STATIC_SAMPLER_DESC desc{};

    desc.ShaderRegister = 0;
    desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
    desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    desc.MinLOD = 0;
    desc.MaxLOD = D3D12_FLOAT32_MAX;
    samplers[static_cast<uint8>(E_SAMPLER_TYPE::WRAP)] = desc;

    desc.ShaderRegister = 1;
    desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    samplers[static_cast<uint8>(E_SAMPLER_TYPE::CLAMP)] = desc;

    LOG("���÷� ��ũ���� ���� ����");
    return true;
}

bool RootSignatureObject::CreateRootSignature()
{
    LOG("��Ʈ �ñ״�ó ���� ����");
    CD3DX12_DESCRIPTOR_RANGE ranges[]
    {
        CD3DX12_DESCRIPTOR_RANGE{D3D12_DESCRIPTOR_RANGE_TYPE_CBV, ENUM_COUNT::CBV_REGISTER - LITERAL::GLOBAL_CBUFFER_COUNT, LITERAL::GLOBAL_CBUFFER_COUNT},
        CD3DX12_DESCRIPTOR_RANGE{D3D12_DESCRIPTOR_RANGE_TYPE_SRV, ENUM_COUNT::SRV_REGISTER, 0},
    };

    CD3DX12_ROOT_PARAMETER param[4];
    param[0].InitAsConstantBufferView(static_cast<uint32>(E_CBV_REGISTER::B0)); // b0
    param[1].InitAsConstantBufferView(static_cast<uint32>(E_CBV_REGISTER::B1)); // b1
    param[2].InitAsConstantBufferView(static_cast<uint32>(E_CBV_REGISTER::B2)); // b2
    param[3].InitAsDescriptorTable(_countof(ranges), ranges);

    D3D12_ROOT_SIGNATURE_DESC desc{ CD3DX12_ROOT_SIGNATURE_DESC{_countof(param), param, static_cast<UINT>(samplers.size()), samplers.data()} };
    desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    Microsoft::WRL::ComPtr<ID3DBlob> blobSignature{};
    Microsoft::WRL::ComPtr<ID3DBlob> blobError{};

    HRESULT hr{};
    hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
    if (FAILED(hr))
    {
        EngineUtil::Shader::CheckBlob(blobError);
        LOG("��Ʈ �ñ״�ó ����ȭ ����");
        return false;
    }
    hr = DEVICE_OBJ->GetDevice()->CreateRootSignature(0,
        blobSignature->GetBufferPointer(),
        blobSignature->GetBufferSize(),
        IID_PPV_ARGS(&signature));
    CHECK(FAILED(hr), "��Ʈ �ñ״�ó ���� ����", false);
    LOG("��Ʈ �ñ״�ó ����");

    LOG("��Ʈ �ñ״�ó ���� ����");
    return true;
}

bool RootSignatureObject::CreateConstantBuffer(E_CBV_REGISTER Register, E_CONSTANT_BUFFER_TYPE BufferType, uint32 BufferSize, uint32 Count)
{
    LOG("�ܽ�źƮ ���� ����");

    uint8 type = static_cast<uint8>(Register);
    assert(constantBuffers.size() == type);
    uint8 idx = static_cast<uint8>(BufferType);
    constantBuffers[idx] = std::make_shared<ConstantBuffer>();
    constantBuffers[idx]->Init(Register, BufferSize, Count);

    LOG("�ܽ�źƮ ���� ���� ����");
    return true;
}
