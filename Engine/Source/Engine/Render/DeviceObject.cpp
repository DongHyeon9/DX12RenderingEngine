#include "DeviceObject.h"

bool DeviceObject::Init()
{
    LOG("����̽� �ʱ�ȭ ����");

	HRESULT hr{};
	//����� ��� Ȱ��ȭ
#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D12Debug> debugController{};
	hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
	CHECK(SUCCEEDED(hr), "����� �������̽� ���� ����", false);
	debugController->EnableDebugLayer();
#endif

	hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	CHECK(SUCCEEDED(hr), "���丮 ���� ����", false);
	LOG("DXGI ���丮 ����");

	hr = D3D12CreateDevice(
		nullptr,					//�⺻ �����
		D3D_FEATURE_LEVEL_11_0,		//Direct3D 11 ������ ��� ���
		IID_PPV_ARGS(&device));

	if (FAILED(hr))
	{
		LOG("�⺻ ����� ����");
		//WARP ����͸� ������
		Microsoft::WRL::ComPtr<IDXGIAdapter> warpAdapter{};
		hr = dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));
		CHECK(SUCCEEDED(hr), "������ ���� ����", false);

		hr = D3D12CreateDevice(
			warpAdapter.Get(),		//WARP ����͸� �̿��� D3D12 ����̽��� ����
			D3D_FEATURE_LEVEL_11_0,	//Direct3D 11 ������ ��� ���
			IID_PPV_ARGS(&device));
		CHECK(SUCCEEDED(hr), "����̽� ���� ����", false);
	}
	LOG("����̽� ����");

#ifdef  _DEBUG
	LogAdpaters();
#endif //  _DEBUG

    LOG("����̽� �ʱ�ȭ ����");
    return true;
}

void DeviceObject::LogAdpaters()
{
	//�����ϴ� ����� Ȯ��
	Microsoft::WRL::ComPtr<IDXGIAdapter> adapter{};
	std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter>> adapterList{};

	for (uint32 i{}; dxgiFactory->EnumAdapters(i, adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_ADAPTER_DESC desc{};
		adapter->GetDesc(&desc);

		LOG("����� �̸� : %s", desc.Description);
		adapterList.emplace_back(adapter);
	}

	for (const auto adt : adapterList)
		LogAdapterOutputs(adt);
}

void DeviceObject::LogAdapterOutputs(Microsoft::WRL::ComPtr<IDXGIAdapter> Adapter)
{
	//��µǴ� ����̽� Ȯ��
	Microsoft::WRL::ComPtr<IDXGIOutput> output{};
	for (uint32 i{}; Adapter->EnumOutputs(i, output.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_OUTPUT_DESC desc{};
		output->GetDesc(&desc);

		LOG("��� ����̽� : %s", desc.DeviceName);
		LogOutputDisplayModes(output);
	}
}

void DeviceObject::LogOutputDisplayModes(Microsoft::WRL::ComPtr<IDXGIOutput> Output)
{
	//����̽����� �����ϴ� �ػ� Ȯ��
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
