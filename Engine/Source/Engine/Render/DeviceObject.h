#pragma once
#include "Header\EngineCore.h"

class DeviceObject
{
public:

protected:

private:
	Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory{};
	Microsoft::WRL::ComPtr<ID3D12Device> device{};

public:
	bool Init();
	FORCEINLINE Microsoft::WRL::ComPtr<ID3D12Device> GetDevice()const { return device; }
	FORCEINLINE Microsoft::WRL::ComPtr<IDXGIFactory4> GetFactory()const { return dxgiFactory; }

protected:

private:
	void LogAdpaters();
	void LogAdapterOutputs(Microsoft::WRL::ComPtr<IDXGIAdapter> Adapter);
	void LogOutputDisplayModes(Microsoft::WRL::ComPtr<IDXGIOutput> Output);

};

