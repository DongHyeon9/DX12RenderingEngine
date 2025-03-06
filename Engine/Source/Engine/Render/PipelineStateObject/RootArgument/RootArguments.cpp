#include "RootArguments.h"

bool RootArguments::CreateSamplerDesc()
{
    LOG("샘플러 디스크립션 생성 시작");

    D3D12_STATIC_SAMPLER_DESC samplerDesc{};

#pragma region Linear Warp
    
    samplerDesc.ShaderRegister = 0;
    samplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.MipLODBias = 0;
    samplerDesc.MaxAnisotropy = 16;
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    samplerDesc.RegisterSpace = 0;

    samplerDescs.emplace_back(samplerDesc);
#pragma endregion Linear Warp

#pragma region Linear Clamp

    samplerDesc.ShaderRegister = 1;
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

    samplerDescs.emplace_back(samplerDesc);
#pragma endregion Linear Clamp

    LOG("샘플러 디스크립션 생성 성공");
    return true;
}

bool RootArguments::CreateRootSignature(Microsoft::WRL::ComPtr<ID3D12Device> Device, const IID& Riid, void** ppvRootSignature)
{
    LOG("루트 시그니처 생성 시작");

    desc.cbvCount = std::clamp(desc.cbvCount, (uint32)0, static_cast<uint32>(E_CBV_REGISTER::END));
    desc.srvCount = std::clamp(desc.srvCount, (uint32)0, static_cast<uint32>(E_SRV_REGISTER::END));
    desc.uavCount = std::clamp(desc.uavCount, (uint32)0, static_cast<uint32>(E_UAV_REGISTER::END));

    const bool bUseTable = desc.cbvTableNum || desc.srvTableNum || desc.uavTableNum;

    uint32 total{
        desc.cbvCount +
        desc.srvCount +
        desc.uavCount };

    total += bUseTable ? 1 : 0;

    std::vector<CD3DX12_ROOT_PARAMETER> param{ total };
    uint32 currentIdx{};
    std::vector<CD3DX12_DESCRIPTOR_RANGE> ranges{};

	for (uint8 i = 0; i < desc.cbvCount; ++i, ++currentIdx)
		param[currentIdx].InitAsConstantBufferView(i);

	for (uint8 i = 0; i < desc.srvCount; ++i, ++currentIdx)
		param[currentIdx].InitAsShaderResourceView(i);

	for (uint8 i = 0; i < desc.uavCount; ++i, ++currentIdx)
        param[currentIdx].InitAsUnorderedAccessView(i);

    if (bUseTable)
    {
        if (desc.cbvTableNum) {
            ranges.emplace_back(CD3DX12_DESCRIPTOR_RANGE{ D3D12_DESCRIPTOR_RANGE_TYPE_CBV, ENUM_COUNT::TABLE_CBV_REGISTER, static_cast<UINT>(E_TABLE_CBV_REGISTER::B5) });
            cbvTable.Init(Device, currentIdx, desc.cbvTableNum);
            tableDescriptorHeaps.emplace_back(cbvTable.GetDescriptorHeap());
        }
        
        if (desc.srvTableNum) {
            ranges.emplace_back(CD3DX12_DESCRIPTOR_RANGE{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, ENUM_COUNT::TABLE_SRV_REGISTER, static_cast<UINT>(E_TABLE_SRV_REGISTER::T5) });
            srvTable.Init(Device, currentIdx, desc.srvTableNum);
            tableDescriptorHeaps.emplace_back(srvTable.GetDescriptorHeap());
        }

        if (desc.uavTableNum) {
            ranges.emplace_back(CD3DX12_DESCRIPTOR_RANGE{ D3D12_DESCRIPTOR_RANGE_TYPE_UAV, ENUM_COUNT::TABLE_UAV_REGISTER, static_cast<UINT>(E_TABLE_UAV_REGISTER::U5) });
            uavTable.Init(Device, currentIdx, desc.uavTableNum);
            tableDescriptorHeaps.emplace_back(uavTable.GetDescriptorHeap());
        }

        if (!ranges.empty())
            param[currentIdx].InitAsDescriptorTable(static_cast<UINT>(ranges.size()), ranges.data());
    }
    
    D3D12_ROOT_SIGNATURE_DESC signatureDesc{
        CD3DX12_ROOT_SIGNATURE_DESC{
            static_cast<UINT>(param.size()),
            param.data(),
            static_cast<UINT>(samplerDescs.size()),
            samplerDescs.data()} };
    signatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    Microsoft::WRL::ComPtr<ID3DBlob> blobSignature{};
    Microsoft::WRL::ComPtr<ID3DBlob> blobError{};

    HRESULT hr{};
    hr = D3D12SerializeRootSignature(&signatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
    if (FAILED(hr))
    {
        EngineUtil::Shader::CheckBlob(blobError);
        LOG("루트 시그니처 직렬화 실패");
        return false;
    }
    LOG("루트 시그니처 직렬화");

    hr = Device->CreateRootSignature(0,
        blobSignature->GetBufferPointer(),
        blobSignature->GetBufferSize(),
        Riid, ppvRootSignature);
    CHECK(SUCCEEDED(hr), "루트 시그니처 생성 실패", false);
    LOG("루트 시그니처 생성");

    LOG("루트 시그니처 생성 성공");
    return true;
}

bool RootArguments::Init(Microsoft::WRL::ComPtr<ID3D12Device> Deivce, const IID& Riid, void** ppvRootSignature, const RootArgumentDesc& Desc)
{
    LOG("루트 아큐먼츠 초기화 시작");

    desc = Desc;
    CHECK(CreateSamplerDesc(), "샘플러 디스크립션 생성 실패", false);
    CHECK(CreateRootSignature(Deivce, Riid, ppvRootSignature), "루트 시그니처 생성 실패", false);

    LOG("루트 아큐먼츠 초기화 성공");
    return true;
}

void RootArguments::SetUAV(E_UAV_REGISTER Register, D3D12_GPU_VIRTUAL_ADDRESS GPUAddress)
{
    uavRegisters[static_cast<uint8>(Register)] = GPUAddress;
}

void RootArguments::SetSRV(E_SRV_REGISTER Register, D3D12_GPU_VIRTUAL_ADDRESS GPUAddress)
{
    srvRegisters[static_cast<uint8>(Register)] = GPUAddress;
}

void RootArguments::SetCBV(E_CBV_REGISTER Register, D3D12_GPU_VIRTUAL_ADDRESS GPUAddress)
{
    cbvRegisters[static_cast<uint8>(Register)] = GPUAddress;
}

void RootArguments::SetUAVTable(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_TABLE_UAV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle)
{
    uavTable.SetData(Device, Register, srcHandle);    
}

void RootArguments::SetSRVTable(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_TABLE_SRV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle)
{
    srvTable.SetData(Device, Register, srcHandle);
}

void RootArguments::SetCBVTable(Microsoft::WRL::ComPtr<ID3D12Device> Device, E_TABLE_CBV_REGISTER Register, D3D12_CPU_DESCRIPTOR_HANDLE srcHandle)
{
    cbvTable.SetData(Device, Register, srcHandle);
}

void RootArguments::CommitData(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CmdList)
{
    uint32 currentIdx{};

    for (const auto& gpuAddress : cbvRegisters)
        CmdList->SetGraphicsRootConstantBufferView(currentIdx++, gpuAddress);

    for (const auto& gpuAddress : srvRegisters)
        CmdList->SetGraphicsRootShaderResourceView(currentIdx++, gpuAddress);

    for (const auto& gpuAddress : uavRegisters)
        CmdList->SetGraphicsRootUnorderedAccessView(currentIdx++, gpuAddress);

    cbvTable.CommitTable(CmdList);
    srvTable.CommitTable(CmdList);
    uavTable.CommitTable(CmdList);
}

void RootArguments::Clear()
{
    cbvTable.Clear();
    srvTable.Clear();
    uavTable.Clear();
}
