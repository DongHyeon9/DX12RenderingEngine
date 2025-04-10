#include "Actor.h"
#include "Component\SceneComp.h"

void Actor::BeginPlay()
{
	if (rootComponent == nullptr)
		rootComponent = std::make_shared<SceneComp>(GetObjectName() + TEXT("Root"));
}

void Actor::Update(float DeltaTime)
{
	ComponentsUpdate(DeltaTime);
	ChildrenUpdate(DeltaTime);
}

void Actor::LateUpdate(float DeltaTime)
{
	rootComponent->LateUpdate(DeltaTime);

	for (const auto& comp : actorComponents)
		comp->LateUpdate(DeltaTime);

	for (const auto& comp : sceneComponents)
		comp->LateUpdate(DeltaTime);

	for (const auto& child : children)
		child->LateUpdate(DeltaTime);
}

void Actor::Render()
{
	rootComponent->Render();

	for (const auto& comp : sceneComponents)
		comp->Render();

	for (const auto& child : children)
		child->Render();
}

void Actor::EndPlay()
{
	__super::EndPlay();

	//현재 부모에 자식들 등록
	auto parentAcotr = parent.lock();
	for (const auto& child : children)
	{
		if (child->IsDestroy())
		{
			child->EndPlay();
		}
		else
		{
			child->parent = parentAcotr;
		}
	}
}

void Actor::SetRootComponent(std::shared_ptr<SceneComp> RootComponent)
{
	RootComponent->SetOwner(std::static_pointer_cast<Actor>(shared_from_this()));
	newRootComp = RootComponent;
}

void Actor::AddActorComponent(std::shared_ptr<Component> NewComp)
{
	NewComp->SetOwner(std::static_pointer_cast<Actor>(shared_from_this()));
	addActorComps.push(NewComp);
}

void Actor::AddSceneComponent(std::shared_ptr<SceneComp> NewComp)
{
	NewComp->SetOwner(std::static_pointer_cast<Actor>(shared_from_this()));
	addSceneComps.push(NewComp);
}

void Actor::AddChild(std::shared_ptr<Actor> Child)
{
	Child->parent = std::static_pointer_cast<Actor>(shared_from_this());
	addChildren.push(Child);
}

Matrix Actor::GetWorldMatrix() const
{
	assert(rootComponent != nullptr);
	Matrix localMat{ rootComponent->GetLocalMatrix() };

	if (parent.expired())
	{
		return localMat;
	}
	else
	{
		auto parentPtr = parent.lock();
		return localMat * parentPtr->GetWorldMatrix();
	}
}

Vector3 Actor::GetRelativePosition() const
{
	return rootComponent->GetRelativePosition();
}

Vector3 Actor::GetRelativeRotation() const
{
	return rootComponent->GetRelativeRotation();
}

Vector3 Actor::GetRelativeScale() const
{
	return rootComponent->GetRelativeScale();
}

Vector3 Actor::GetActorPosition() const
{
	return rootComponent->GetWorldPosition();
}

Vector3 Actor::GetActorRotation() const
{
	return rootComponent->GetWorldRotation();
}

Vector3 Actor::GetActorScale() const
{
	return rootComponent->GetWorldScale();
}

void Actor::SetRelativeTransform(const Transform& Transform)
{
	rootComponent->SetRelativeTransform(Transform);
}

void Actor::SetRelativePosition(const Vector3& Position)
{
	rootComponent->SetRelativePosition(Position);
}

void Actor::SetRelativeRotation(const Vector3& Rotation)
{
	rootComponent->SetRelativeRotation(Rotation);
}

void Actor::SetRelativeScale(const Vector3& Scale)
{
	rootComponent->SetRelativeScale(Scale);
}

void Actor::SetWorldTransform(const Transform& Transform)
{
	SetWorldPosition(Transform.position);
	SetWorldRotation(Transform.rotation);
	SetWorldScale(Transform.scale);
}

void Actor::SetWorldPosition(const Vector3& Position)
{
	if (parent.expired())
	{
		SetRelativePosition(Position);
	}
	else
	{
		Matrix inverseMat = parent.lock()->GetWorldMatrix().Invert();

		Vector3 position{};
		position = Vector3::Transform(Position, inverseMat);

		SetRelativePosition(position);
	}
}

void Actor::SetWorldRotation(const Vector3& Rotation)
{
	if (parent.expired())
	{
		SetRelativeRotation(Rotation);
	}
	else
	{
		Matrix inverseMat = parent.lock()->GetWorldMatrix().Invert();

		Vector3 rotation{};
		rotation = Vector3::TransformNormal(Rotation, inverseMat);

		SetRelativeRotation(rotation);
	}
}

void Actor::SetWorldScale(const Vector3& Scale)
{
	if (parent.expired())
	{
		SetRelativeScale(Scale);
	}
	else
	{
		Vector3 parentScale = parent.lock()->GetActorScale();
		Vector3 scale = Scale;
		scale.x /= parentScale.x;
		scale.y /= parentScale.y;
		scale.z /= parentScale.z;
		SetRelativeScale(scale);
	}
}
void Actor::ChildrenUpdate(float DeltaTime)
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

	for (const auto& child : children)
	{
		child->Update(DeltaTime);
	}
}

void Actor::ComponentsUpdate(float DeltaTime)
{
	//액터 컴포넌트 제거
	for (auto iter = actorComponents.begin(); iter != actorComponents.end(); ++iter)
	{
		auto comp = *iter;
		if (comp->IsDestroy())
		{
			comp->EndPlay();
			iter = actorComponents.erase(iter);
		}
	}

	//씬 컴포넌트 제거
	for (auto iter = sceneComponents.begin(); iter != sceneComponents.end(); ++iter)
	{
		auto comp = *iter;
		if (comp->IsDestroy())
		{
			comp->EndPlay();
			iter = sceneComponents.erase(iter);
		}
	}

	//액터 컴포넌트 추가
	while (!addActorComps.empty())
	{
		auto comp = addActorComps.front();
		comp->BeginPlay();
		actorComponents.emplace_back(comp);
		addActorComps.pop();
	}

	//씬 컴포넌트 추가
	while (!addSceneComps.empty())
	{
		auto comp = addSceneComps.front();
		comp->BeginPlay();
		sceneComponents.emplace_back(comp);
		addSceneComps.pop();
	}

	//루트 컴포넌트 재설정
	if (newRootComp != nullptr)
	{
		rootComponent = newRootComp;
		rootComponent->BeginPlay();
		newRootComp.reset();
	}

	assert(rootComponent != nullptr);
	rootComponent->Update(DeltaTime);

	for (const auto& comp : actorComponents)
	{
		comp->Update(DeltaTime);
	}

	for (const auto& comp : sceneComponents)
	{
		comp->Update(DeltaTime);
	}
}
