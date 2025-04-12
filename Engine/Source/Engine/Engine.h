#pragma once
#include "Header\EngineCore.h"

class AppBase;

class Engine
{
public:

protected:

private:
	std::shared_ptr<AppBase> app{};
	EngineSetting engineSetting{};

	Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer{};

public:
	bool Init(const EngineDesc& EngineDescription);
	WPARAM Run();

	FORCEINLINE std::shared_ptr<AppBase> GetApp()const { return app; }
	FORCEINLINE const EngineSetting& GetEngineSetting()const { return engineSetting; }

protected:

private:
	bool InitWindow();
	bool InitManager();

	ATOM EngineRegisterClass();
	BOOL InitInstance(int CmdShow);
	bool InitEngineSetting();

	void EngineUpdate();
	void Update(float DeltaTime);
	void Render();
};

static LRESULT CALLBACK WndProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);