#pragma once
#include "Object\Object.h"

class Scene;
class Component;
class SceneComp;

class Actor : public Object
{
public:

protected:

private:
	bool bIsDestroyFlag{};

	std::weak_ptr<Scene> currentScene{};
	std::weak_ptr<Actor> parent{};
	std::vector<std::shared_ptr<Actor>> children{};

	std::shared_ptr<SceneComp> rootComponent{};
	std::vector<std::shared_ptr<Component>> actorComponents{};
	std::vector<std::shared_ptr<SceneComp>> sceneComponents{};

	std::shared_ptr<SceneComp> newRootComp{};
	std::queue<std::shared_ptr<Component>> addActorComps{};
	std::queue<std::shared_ptr<SceneComp>> addSceneComps{};
	std::queue<std::shared_ptr<Actor>> addChildren{};

public:
	Actor(FString ObjName) :Object(std::move(ObjName)) {}
	virtual void BeginPlay();
	virtual void Update(float DeltaTime);
	virtual void LateUpdate(float DeltaTime);
	virtual void Render();

	void EndPlay()override;
	void SetRootComponent(std::shared_ptr<SceneComp> RootComponent);
	void Init(std::shared_ptr<SceneComp> RootComponent);
	void AddActorComponent(std::shared_ptr<Component> NewComp);
	void AddSceneComponent(std::shared_ptr<SceneComp> NewComp);
	void AddChild(std::shared_ptr<Actor> Child);

	Matrix GetWorldMatrix()const;

	FORCEINLINE const std::vector<std::shared_ptr<Actor>>& GetChildren()const { return children; }
	FORCEINLINE std::shared_ptr<Actor> GetParent()const { if (parent.expired()) return nullptr; else return parent.lock(); }

	FORCEINLINE std::shared_ptr<SceneComp> GetRootComponent()const { return rootComponent; }
	FORCEINLINE bool HasParent()const { return !parent.expired(); }

	FORCEINLINE Transform GetRelativeTransform()const { return Transform{ GetRelativePosition(), GetRelativeRotation(), GetRelativeScale() }; }
	Vector3 GetRelativePosition()const;
	Vector3 GetRelativeRotation()const;
	Vector3 GetRelativeScale()const;

	FORCEINLINE Transform GetActorTransform()const { return Transform{ GetActorPosition(), GetActorRotation(), GetActorScale() }; }
	Vector3 GetActorPosition()const;
	Vector3 GetActorRotation()const;
	Vector3 GetActorScale()const;

	void SetRelativeTransform(const Transform& Transform);
	void SetRelativePosition(const Vector3& Position);
	void SetRelativeRotation(const Vector3& Rotation);
	void SetRelativeScale(const Vector3& Scale);

	void SetWorldTransform(const Transform& Transform);
	void SetWorldPosition(const Vector3& Position);
	void SetWorldRotation(const Vector3& Rotation);
	void SetWorldScale(const Vector3& Scale);

protected:

private:
	void ChildrenUpdate(float DeltaTime);
	void ComponentsUpdate(float DeltaTime);

};

