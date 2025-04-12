#include "Texture.h"

#include "Manager\RenderManager.h"
#include "Engine\Render\DeviceObject.h"
#include "Engine\Render\CommandObject.h"

bool Texture::Load(FString FileName)
{
    __super::Load(FileName);
	LOG("%s �ؽ�ó ���� ����", fileName.c_str());

	Microsoft::WRL::ComPtr<ID3D12Device> device = DEVICE_OBJ->GetDevice();
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList = CMD_OBJ->GetResourceCommandList();

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

	CMD_OBJ->ExecuteResourceCommandList();

	LOG("%s �ؽ�ó ���� ����", fileName.c_str());
    return true;
}

bool Texture::CreateView(D3D12_SRV_DIMENSION TextureDimension)
{
	LOG("���̴� ���ҽ� �� ���� ����");

	Microsoft::WRL::ComPtr<ID3D12Device> device = DEVICE_OBJ->GetDevice();

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));

	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = image.GetMetadata().format;
	srvDesc.ViewDimension = TextureDimension;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = 1;
	device->CreateShaderResourceView(texture2D.Get(), &srvDesc, srvHandle);

	LOG("���̴� ���ҽ� �� ���� ����");
	return true;
}
