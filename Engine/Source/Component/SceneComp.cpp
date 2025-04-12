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
	//�ڽ� ����
	for (auto iter = children.begin(); iter != children.end(); ++iter)
	{
		auto child = *iter;
		if (child->IsDestroy())
		{
			child->EndPlay();
			iter = children.erase(iter);
		}
	}

	//�ڽ� �߰�
	while (!addChildren.empty())
	{
		children.emplace_back(addChildren.front());
		addChildren.pop();
	}

	//Ʈ������ ���� ����
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
	//������->ȸ��->�̵��� ������ ����� ���ؼ� ������ش�
	local =
		Matrix::CreateScale(relativeTransform.scale) *
		Matrix::CreateRotationX(relativeTransform.rotation.x) *
		Matrix::CreateRotationY(relativeTransform.rotation.y) *
		Matrix::CreateRotationZ(relativeTransform.rotation.z) *
		Matrix::CreateTranslation(relativeTransform.position);

	//�θ�������Ʈ�� �ִٸ� �θ�������Ʈ�� ��İ� �����ϰ�
	//�θ� ���ٸ� ���� ������Ʈ�� ������ Ʈ���������� Ȯ���ϰ�
	//�´ٸ� ������ �θ� ���� ��İ��
	//�ƴ϶�� ������ Ʈ������ ������Ʈ�� ���� ��İ��
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

	//������Ŀ��� ���� Ʈ�������� �����´�
	Quaternion quat{};
	world.worldMat.Decompose(worldTransform.scale, quat, worldTransform.position);
	worldTransform.rotation = EngineUtil::Math::ToEulerAngles(quat);

	//SimpleMath�� ������ ��ǥ�� �����̶� Forward�� Backward�� �ݴ�� �����ؾߵ�
	forward = Vector3::TransformNormal(Vector3::Backward, world.worldMat);
	right = Vector3::TransformNormal(Vector3::Right, world.worldMat);
	up = Vector3::TransformNormal(Vector3::Up, world.worldMat);

	for (auto child : children)
		child->UpdateTransform();
}
