#include "Scene.h"

#include "Manager\GeometryManager.h"
#include "Manager\ResourceManager.h"

#include "Resource\External\Texture.h"
#include "Resource\External\Mesh.h"
#include "Resource\Internal\Material.h"
#include "Object\Actor.h"
#include "Component\MeshComp.h"

bool Scene::Init()
{
    LOG("씬 초기화 시작");

    //TODO 메쉬 생성
    std::shared_ptr<Actor> actor{ std::make_shared<Actor>(TEXT("Cube")) };
    std::shared_ptr<MeshComp> meshComp{ std::make_shared<MeshComp>(TEXT("CubeRoot")) };
    std::shared_ptr<Material> material{ std::make_shared<Material>() };
    std::shared_ptr<Texture> albedo = ResourceManager::GetInstance()->GetResource<Texture>(TEXT("T_Earth.jpg"));
    albedo->CreateView();
    material->SetTexture(E_TEXTURE_TYPE::ALBEDO, albedo);
    meshComp->Init(GeometryManager::GetInstance()->CreateBox(Vector3{ 100.0f,100.0f,100.0f }));
    meshComp->SetMaterial(0, material);
    actor->Init(meshComp);
    //actor->SetWorldPosition(Vector3{ 0.0f,0.0f,500.0f });

    children.emplace_back(actor);

    LOG("씬 초기화 성공");
    return true;
}

void Scene::OnEnterScene()
{
}

void Scene::OnExitScene()
{
}

void Scene::Update(float DeltaTime)
{
    //자식 제거
    for (auto iter = children.begin(); iter != children.end(); ++iter)
    {
        auto child = *iter;
        if (child->IsDestroy())
        {
            child->EndPlay();
            iter = children.erase(iter);
        }
    }

    //자식 추가
    while (!addChildren.empty())
    {
        auto child = addChildren.front();
        child->BeginPlay();
        children.emplace_back(child);
        addChildren.pop();
    }

    for (auto child : children)
    {
        child->Update(DeltaTime);
    }
}

void Scene::LateUpdate(float DeltaTime)
{
    for (auto child : children)
        child->LateUpdate(DeltaTime);
}

void Scene::Render()
{
    for (auto child : children)
        child->Render();
}

void Scene::AddActor(std::shared_ptr<Actor> Actor)
{
    addChildren.push(Actor);
}
