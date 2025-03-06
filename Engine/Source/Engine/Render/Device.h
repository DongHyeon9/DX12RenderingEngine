#pragma once
#include "Header\EngineCoreMinimal.h"

class Device
{
public:

protected:

private:
	//ȭ�鿡 ���õ� ��ɵ��� ����(�����Ǵ� ���÷��� ��� ���� ��)
	Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory{};
	//���÷��� �����(�ϵ����, ����Ʈ����)�� ��Ÿ���� ��ü
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

