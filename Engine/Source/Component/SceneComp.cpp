#include "SceneComp.h"
#include "Object\Actor.h"

#include "Manager\RenderManager.h"
#include "Engine\Render\PipelineStateObject.h"
#include "Engine\Render\PipelineStateObject\RootSignatureObject.h"

void SceneComp::EndPlay()
{
	__super::EndPlay();

	if (parent.expired())
	{
		if (owner.expired())
			return;

		for (const auto& child : children)
		{
			if (child->IsDestroy())
			{
				child->EndPlay();
			}
			else
			{
				auto actor = owner.lock();
				actor->AddSceneComponent(child);
			}
		}
	}
	else
	{
		auto newParent = parent.lock();
		for (const auto& child : children)
		{
			if (child->IsDestroy())
			{
				child->EndPlay();
			}
			else
			{
				newParent->AddChild(child);
			}
		}
	}
}

void SceneComp::Update(float DeltaTime)
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
		children.emplace_back(addChildren.front());
		addChildren.pop();
	}

	//트랜스폼 변경 로직
	UpdateTransform();

	for (const auto& child : children)
	{
		child->Update(DeltaTime);
	}
}

void SceneComp::LateUpdate(float DeltaTime)
{
	for (const auto& child : children)
	{
		child->LateUpdate(DeltaTime);
	}
}

void SceneComp::Render()
{
	for (const auto& child : children)
	{
		child->Render();
	}

	PSO->GetRootSignature()->PushData(E_CONSTANT_BUFFER_TYPE::TRANSFORM, &world, sizeof(world));
}

void SceneComp::AddChild(std::shared_ptr<SceneComp> NewChild)
{
	NewChild->parent = std::static_pointer_cast<SceneComp>(shared_from_this());
	addChildren.push(NewChild);
}

void SceneComp::SetRelativeTransform(const Transform& Transform)
{
	relativeTransform = Transform;
	UpdateTransform();
}

void SceneComp::SetRelativePosition(const Vector3& Position)
{
	relativeTransform.position = Position / GLOBAL::UNIT;
	UpdateTransform();
}

void SceneComp::SetRelativeRotation(const Vector3& Rotation)
{
	relativeTransform.rotation = Rotation;
	UpdateTransform();
}

void SceneComp::SetRelativeScale(const Vector3& Scale)
{
	relativeTransform.scale = Scale;
	UpdateTransform();
}

void SceneComp::SetWorldTransform(const Transform& Transform)
{
	SetWorldPosition(Transform.position);
	SetWorldRotation(Transform.rotation);
	SetWorldScale(Transform.scale);
}

void SceneComp::SetWorldPosition(const Vector3& Position)
{
	if (parent.expired())
	{
		SetRelativePosition(Position);
	}
	else
	{
		Matrix inverseMat = parent.lock()->world.inverseWorldMat;

		Vector3 position{};
		position.Transform(Position, inverseMat);

		SetRelativePosition(position);
	}
}

void SceneComp::SetWorldRotation(const Vector3& Rotation)
{
	if (parent.expired())
	{
		SetRelativeRotation(Rotation);
	}
	else
	{
		Matrix inverseMat = parent.lock()->world.inverseWorldMat;
		Vector3 rotation{};
		rotation.TransformNormal(Rotation, inverseMat);
		SetRelativeRotation(rotation);
	}
}

void SceneComp::SetWorldScale(const Vector3& Scale)
{
	if (parent.expired())
	{
		SetRelativeScale(Scale);
	}
	else
	{
		Vector3 parentScale = parent.lock()->GetWorldScale();
		Vector3 scale = Scale;
		scale.x /= parentScale.x;
		scale.y /= parentScale.y;
		scale.z /= parentScale.z;
		SetRelativeScale(scale);
	}
}

void SceneComp::UpdateTransform()
{
	//스케일->회전->이동의 순서로 행렬을 곱해서 만들어준다
	local =
		Matrix::CreateScale(relativeTransform.scale) *
		Matrix::CreateRotationX(relativeTransform.rotation.x) *
		Matrix::CreateRotationY(relativeTransform.rotation.y) *
		Matrix::CreateRotationZ(relativeTransform.rotation.z) *
		Matrix::CreateTranslation(relativeTransform.position);

	//부모컴포넌트가 있다면 부모컴포넌트의 행렬과 곱을하고
	//부모가 없다면 현재 컴포넌트가 오너의 트랜스폼인지 확인하고
	//맞다면 오너의 부모에 따라 행렬계산
	//아니라면 오너의 트렌스폼 컴포넌트에 따라 행렬계산
	if (parent.expired())
	{
		assert(!owner.expired());
		auto ownerPtr = owner.lock();
		auto ownerTransform = ownerPtr->GetRootComponent();

		if (ownerTransform.get() == this)
		{
			world.worldMat = ownerPtr->GetWorldMatrix();
		}
		else
		{
			world.worldMat = local * ownerTransform->world.worldMat;
		}
	}
	else
	{
		auto parentPtr = parent.lock();
		world.worldMat = local * parentPtr->world.worldMat;
	}

	world.inverseWorldMat = world.worldMat.Invert();

	//월드행렬에서 월드 트랜스폼을 가져온다
	Quaternion quat{};
	world.worldMat.Decompose(worldTransform.scale, quat, worldTransform.position);
	worldTransform.rotation = EngineUtil::Math::ToEulerAngles(quat);

	//SimpleMath가 오른손 좌표계 기준이라 Forward랑 Backward를 반대로 적용해야됨
	forward = Vector3::TransformNormal(Vector3::Backward, world.worldMat);
	right = Vector3::TransformNormal(Vector3::Right, world.worldMat);
	up = Vector3::TransformNormal(Vector3::Up, world.worldMat);

	for (auto child : children)
		child->UpdateTransform();
}
