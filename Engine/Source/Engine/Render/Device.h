#pragma once
#include "Header\EngineCoreMinimal.h"

class Device
{
public:

protected:

private:
	//화면에 관련된 기능들을 제공(지원되는 디스플레이 모드 열거 등)
	Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory{};
	//디스플레이 어댑터(하드웨어, 소프트웨어)를 나타내는 객체
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

