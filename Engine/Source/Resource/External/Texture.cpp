#include "Texture.h"
#include "Engine/Engine.h"

bool Texture::Load(FString FileName)
{
    __super::Load(FileName);
	LOG("%s 텍스처 생성 시작", fileName.c_str());

	auto device = GEngine->GetDevice();
	auto cmdList = GEngine->GetCommandList();

	FString fullPath = path + fileName;
	FString ext = std::filesystem::path(fullPath).extension();

	HRESULT hr{};

	//텍스처 파일 읽어들이기
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

	CHECK(SUCCEEDED(hr), "유효하지 않은 경로의 텍스처", false);

	//로드된 텍스처를 기반으로 ID3D12Resource 생성
	hr = DirectX::CreateTexture(device.Get(), image.GetMetadata(), &texture2D);

	CHECK(SUCCEEDED(hr), "텍스처 생성 실패", false);

	//GPU에 업로드하기 위한 서브리소스 리스트
	std::vector<D3D12_SUBRESOURCE_DATA> subResources{};

	//로드한 이미지 데이터를 GPU 업로드를 위해 변환
	hr = DirectX::PrepareUpload(device.Get(),
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		subResources);

	CHECK(SUCCEEDED(hr), "업로드 변환 실패", false);

	const uint64 bufferSize{ GetRequiredIntermediateSize(texture2D.Get(), 0, static_cast<uint32>(subResources.size())) };

	D3D12_HEAP_PROPERTIES heapProperty{ CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_UPLOAD } };
	D3D12_RESOURCE_DESC desc{ CD3DX12_RESOURCE_DESC::Buffer(bufferSize) };

	Microsoft::WRL::ComPtr<ID3D12Resource> uploadHeap{};

	//업로드를 위한 리소스를 생성
	hr = device->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(uploadHeap.GetAddressOf())
	);

	CHECK(SUCCEEDED(hr), "리소스 커밋 실패", false);

	//생성한 텍스처 데이터(uploadHeap)를 texture2D 리소스로 복사
	UpdateSubresources(
		cmdList.Get(),
		texture2D.Get(),
		uploadHeap.Get(),
		0, 0,
		static_cast<uint32>(subResources.size()),
		subResources.data());

	LOG("%s 텍스처 생성 성공", fileName.c_str());
    return true;
}