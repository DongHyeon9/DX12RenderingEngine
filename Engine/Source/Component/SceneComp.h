#pragma once
#include "Component\Component.h"

class SceneComp : public Component
{
public:

protected:
	std::weak_ptr<SceneComp> parent{};
	std::vector<std::shared_ptr<SceneComp>> children{};

private:
	Transform relativeTransform{};
	Transform worldTransform{};
	TransformMatrix world{};
	Matrix local{ Matrix::Identity };

	Vector3 forward{};
	Vector3 right{};
	Vector3 up{};

	std::queue<std::shared_ptr<SceneComp>> addChildren{};

public:
	SceneComp(FString ObjName) :Component(std::move(ObjName)) {}
	void EndPlay()override;
	void Update(float DeltaTime)override;
	void LateUpdate(float DeltaTime)override;
	virtual void Render();

	void AddChild(std::shared_ptr<SceneComp> NewChild);

	FORCEINLINE Vector3 GetForwardVector()const { return forward; }
	FORCEINLINE Vector3 GetRightVector()const { return right; }
	FORCEINLINE Vector3 GetUpVector()const { return up; }
	FORCEINLINE Matrix GetLocalMatrix()const { return local; }
	FORCEINLINE Matrix GetWorldMatrix()const { return world.worldMat; }

	FORCEINLINE Transform GetRelativeTransform()const { return Transform{ GetRelativePosition(), GetRelativeRotation(), GetRelativeScale() }; }
	FORCEINLINE Vector3 GetRelativePosition()const { return relativeTransform.position * GLOBAL::UNIT; }
	FORCEINLINE Vector3 GetRelativeRotation()const { return relativeTransform.rotation; }
	FORCEINLINE Vector3 GetRelativeScale()const { return relativeTransform.scale; }

	FORCEINLINE Transform GetWorldTransform()const { return Transform{ GetWorldPosition(), GetWorldRotation(), GetWorldScale() }; }
	FORCEINLINE Vector3 GetWorldPosition()const { return worldTransform.position * GLOBAL::UNIT; }
	FORCEINLINE Vector3 GetWorldRotation()const { return worldTransform.rotation; }
	FORCEINLINE Vector3 GetWorldScale()const { return worldTransform.scale; }

	void SetRelativeTransform(const Transform& Transform);
	void SetRelativePosition(const Vector3& Position);
	void SetRelativeRotation(const Vector3& Rotation);
	void SetRelativeScale(const Vector3& Scale);

	void SetWorldTransform(const Transform& Transform);
	void SetWorldPosition(const Vector3& Position);
	void SetWorldRotation(const Vector3& Rotation);
	void SetWorldScale(const Vector3& Scale);

protected:
	virtual void UpdateTransform();

private:

};

