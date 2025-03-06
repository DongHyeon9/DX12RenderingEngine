#include "AppBase.h"
#include "Scene.h"

bool AppBase::Init()
{
    LOG("앱 초기화 시작");

    scene = std::make_shared<Scene>();
    scene->OnEnterScene();

    CHECK(scene->Init(), "씬 초기화 실패", false);

    LOG("앱 초기화 성공");
    return true;
}

void AppBase::SetScene(std::shared_ptr<Scene> NewScene)
{
    if (scene != nullptr)
        scene->OnExitScene();

    scene = NewScene;
    scene->Init();
    scene->OnEnterScene();
}

void AppBase::Update(float DeltaTime)
{
    if (scene != nullptr)
        scene->Update(DeltaTime);
}

void AppBase::LateUpdate(float DeltaTime)
{
    if (scene != nullptr)
        scene->LateUpdate(DeltaTime);
}

void AppBase::Render()
{
    if (scene != nullptr)
        scene->Render();
}

LRESULT AppBase::WndProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}
