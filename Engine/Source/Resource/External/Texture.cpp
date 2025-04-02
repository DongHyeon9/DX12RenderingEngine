#include "Texture.h"
#include "Engine/Engine.h"

bool Texture::Load(FString FileName)
{
    __super::Load(FileName);
	LOG("%s �ؽ�ó ���� ����", fileName.c_str());

	auto device = GEngine->GetDevice();
	auto cmdList = GEngine->GetCommandList();

	FString fullPath = path + fileName;
	FString ext = std::filesystem::path(fullPath).extension();

	HRESULT hr{};

	//�ؽ�ó ���� �о���̱�
	if (ext == TEXT(".dds") || ext == TEXT(".DDS"))
		hr = DirectX::LoadFromDDSFile(
			EngineUtil::String::ConvertString<std::wstring>(fullPath).c_str(),
			DirectX::DDS_FLAGS_NONE, nullptr, image);

	else if (ext == TEXT(".tga") || ext == TEXT(".TGA"))
		hr = DirectX::LoadFromTGAFile(
			EngineUtil::String::ConvertString<std::wstring>(fullPath).c_str(),
			nullptr, image);

	else
		hr = DirectX::LoadFromWICFile(
			EngineUtil::String::ConvertString<std::wstring>(fullPath).c_str(),
			DirectX::WIC_FLAGS_NONE, nullptr, image);

	CHECK(SUCCEEDED(hr), "��ȿ���� ���� ����� �ؽ�ó", false);

	//�ε�� �ؽ�ó�� ������� ID3D12Resource ����
	hr = DirectX::CreateTexture(device.Get(), image.GetMetadata(), &texture2D);

	CHECK(SUCCEEDED(hr), "�ؽ�ó ���� ����", false);

	//GPU�� ���ε��ϱ� ���� ���긮�ҽ� ����Ʈ
	std::vector<D3D12_SUBRESOURCE_DATA> subResources{};

	//�ε��� �̹��� �����͸� GPU ���ε带 ���� ��ȯ
	hr = DirectX::PrepareUpload(device.Get(),
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		subResources);

	CHECK(SUCCEEDED(hr), "���ε� ��ȯ ����", false);

	const uint64 bufferSize{ GetRequiredIntermediateSize(texture2D.Get(), 0, static_cast<uint32>(subResources.size())) };

	D3D12_HEAP_PROPERTIES heapProperty{ CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_UPLOAD } };
	D3D12_RESOURCE_DESC desc{ CD3DX12_RESOURCE_DESC::Buffer(bufferSize) };

	Microsoft::WRL::ComPtr<ID3D12Resource> uploadHeap{};

	//���ε带 ���� ���ҽ��� ����
	hr = device->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(uploadHeap.GetAddressOf())
	);

	CHECK(SUCCEEDED(hr), "���ҽ� Ŀ�� ����", false);

	//������ �ؽ�ó ������(uploadHeap)�� texture2D ���ҽ��� ����
	UpdateSubresources(
		cmdList.Get(),
		texture2D.Get(),
		uploadHeap.Get(),
		0, 0,
		static_cast<uint32>(subResources.size()),
		subResources.data());

	LOG("%s �ؽ�ó ���� ����", fileName.c_str());
    return true;
}