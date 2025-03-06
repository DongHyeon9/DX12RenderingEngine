#include "AppBase.h"
#include "Scene.h"

bool AppBase::Init()
{
    LOG("�� �ʱ�ȭ ����");

    scene = std::make_shared<Scene>();
    scene->OnEnterScene();

    CHECK(scene->Init(), "�� �ʱ�ȭ ����", false);

    LOG("�� �ʱ�ȭ ����");
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
