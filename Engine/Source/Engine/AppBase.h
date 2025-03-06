#pragma once
#include "Header\EngineCore.h"

class Scene;

class AppBase
{
public:

protected:
	std::shared_ptr<Scene> scene{};

private:

public:
	virtual bool Init();
	void SetScene(std::shared_ptr<Scene> NewScene);

	virtual void Update(float DeltaTime);
	virtual void LateUpdate(float DeltaTime);
	virtual void Render();

	virtual LRESULT CALLBACK WndProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);
	FORCEINLINE std::shared_ptr<Scene> GetCurrentScene()const { return scene; }

protected:

private:

};

