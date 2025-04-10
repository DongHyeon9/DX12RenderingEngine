#include "DeviceObject.h"

bool DeviceObject::Init()
{
    LOG("디바이스 초기화 시작");

	HRESULT hr{};
	//디버그 모드 활성화
#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D12Debug> debugController{};
	hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
	CHECK(SUCCEEDED(hr), "디버그 인터페이스 생성 실패", false);
	debugController->EnableDebugLayer();
#endif

	hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	CHECK(SUCCEEDED(hr), "팩토리 생성 실패", false);
	LOG("DXGI 팩토리 생성");

	hr = D3D12CreateDevice(
		nullptr,					//기본 어댑터
		D3D_FEATURE_LEVEL_11_0,		//Direct3D 11 수준의 기능 사용
		IID_PPV_ARGS(&device));

	if (FAILED(hr))
	{
		LOG("기본 어댑터 없음");
		//WARP 어댑터를 가져옴
		Microsoft::WRL::ComPtr<IDXGIAdapter> warpAdapter{};
		hr = dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));
		CHECK(SUCCEEDED(hr), "렌더링 지원 안함", false);

		hr = D3D12CreateDevice(
			warpAdapter.Get(),		//WARP 어댑터를 이용해 D3D12 디바이스를 생성
			D3D_FEATURE_LEVEL_11_0,	//Direct3D 11 수준의 기능 사용
			IID_PPV_ARGS(&device));
		CHECK(SUCCEEDED(hr), "디바이스 생성 실패", false);
	}
	LOG("디바이스 생성");

#ifdef  _DEBUG
	LogAdpaters();
#endif //  _DEBUG

    LOG("디바이스 초기화 성공");
    return true;
}

void DeviceObject::LogAdpaters()
{
	//지원하는 어댑터 확인
	Microsoft::WRL::ComPtr<IDXGIAdapter> adapter{};
	std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter>> adapterList{};

	for (uint32 i{}; dxgiFactory->EnumAdapters(i, adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_ADAPTER_DESC desc{};
		adapter->GetDesc(&desc);

		LOG("어댑터 이름 : %s", desc.Description);
		adapterList.emplace_back(adapter);
	}

	for (const auto adt : adapterList)
		LogAdapterOutputs(adt);
}

void DeviceObject::LogAdapterOutputs(Microsoft::WRL::ComPtr<IDXGIAdapter> Adapter)
{
	//출력되는 디바이스 확인
	Microsoft::WRL::ComPtr<IDXGIOutput> output{};
	for (uint32 i{}; Adapter->EnumOutputs(i, output.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_OUTPUT_DESC desc{};
		output->GetDesc(&desc);

		LOG("출력 디바이스 : %s", desc.DeviceName);
		LogOutputDisplayModes(output);
	}
}

void DeviceObject::LogOutputDisplayModes(Microsoft::WRL::ComPtr<IDXGIOutput> Output)
{
	//디바이스에서 지원하는 해상도 확인
	uint32 count{};
	uint32 flags{};

	Output->GetDisplayModeList(LITERAL::BACK_BUFFER_FORMAT, flags, &count, nullptr);

	std::vector<DXGI_MODE_DESC> modeList{ count };
	Output->GetDisplayModeList(LITERAL::BACK_BUFFER_FORMAT, flags, &count, modeList.data());

	for (auto& mode : modeList)
	{
		LOG("{ Width : %d } { Height : %d } { Refresh : %d / %d }",
			mode.Width,
			mode.Height,
			mode.RefreshRate.Numerator,
			mode.RefreshRate.Denominator);
	}
}
