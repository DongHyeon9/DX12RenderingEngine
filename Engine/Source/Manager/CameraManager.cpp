#include "CameraManager.h"
#include "InputManager.h"

#include "Manager\RenderManager.h"
#include "Engine\Render\PipelineStateObject.h"
#include "Engine\Render\PipelineStateObject\RootSignatureObject.h"

bool CameraManager::Init()
{
	return true;
}

void CameraManager::Update(float DeltaTime)
{
	CameraMove(DeltaTime);
	UpdateTransform();
}

void CameraManager::Render()
{
	PSO->GetRootSignature()->PushGlobalData(E_CONSTANT_BUFFER_TYPE::CAMERA, &cameraData, sizeof(cameraData));
}

void CameraManager::SetCameraLocation(const Vector3& NewLocation)
{
	cameraPosition = NewLocation / GLOBAL::UNIT;
	UpdateTransform();
}

void CameraManager::SetCameraRotation(const Vector3& NewRotation)
{
	cameraRotation = NewRotation;
	UpdateTransform();
}

void CameraManager::CameraMove(float DeltaTime)
{
	float moveDistance = DeltaTime * cameraMoveSpeed;
	if (KEY_PRESS(E_KEY_TYPE::W))
	{
		cameraPosition += forward * moveDistance;
	}
	if (KEY_PRESS(E_KEY_TYPE::S))
	{
		cameraPosition -= forward * moveDistance;
	}
	if (KEY_PRESS(E_KEY_TYPE::A))
	{
		cameraPosition -= right * moveDistance;
	}
	if (KEY_PRESS(E_KEY_TYPE::D))
	{
		cameraPosition += right * moveDistance;
	}
	if (KEY_PRESS(E_KEY_TYPE::Q))
	{
		cameraPosition -= up * moveDistance;
	}
	if (KEY_PRESS(E_KEY_TYPE::E))
	{
		cameraPosition += up * moveDistance;
	}

	if (KEY_PRESS(E_KEY_TYPE::L_ALT) && KEY_DOWN(E_KEY_TYPE::L_BUTTON))
	{
		currentMousePos = InputManager::GetInstance()->GetMousePos();
	}
	if (KEY_PRESS(E_KEY_TYPE::L_ALT) && KEY_PRESS(E_KEY_TYPE::L_BUTTON))
	{
		float turnDistance = cameraTurnSpeed * DeltaTime;
		POINT mousePos = InputManager::GetInstance()->GetMousePos();
		POINT diff{ currentMousePos.y - mousePos.y, currentMousePos.x - mousePos.x };
		cameraRotation.y -= diff.y * turnDistance;
		cameraRotation.x -= diff.x * turnDistance;
		currentMousePos = mousePos;
	}
}

void CameraManager::UpdateTransform()
{
	Matrix rotationMat = Matrix::CreateRotationX(cameraRotation.x) *
		Matrix::CreateRotationY(cameraRotation.y) *
		Matrix::CreateRotationZ(cameraRotation.z);

	cameraData.view = rotationMat * Matrix::CreateTranslation(cameraPosition);

	forward = Vector3::TransformNormal(Vector3::Backward, rotationMat);
	right = Vector3::TransformNormal(Vector3::Right, rotationMat);
	up = Vector3::TransformNormal(Vector3::Up, rotationMat);

	cameraData.view = cameraData.view.Invert();

	cameraData.cameraPos = Vector4(cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f);

	const float aspect = static_cast<float>(RESOLUTION::WIDTH) / RESOLUTION::HEIGHT;
	//cameraData.projection =
	//	GEngine->GetRenderSetting().bIsOrthographic ?
	//	DirectX::XMMatrixOrthographicOffCenterLH(-aspect, aspect, -1.0f, 1.0f, nearPlane, farPlane) :
	//	DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(FOV), aspect, nearPlane, farPlane);
	
	cameraData.projection = DirectX::XMMatrixOrthographicOffCenterLH(-aspect, aspect, -1.0f, 1.0f, nearPlane, farPlane);
	//cameraData.projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(FOV), aspect, nearPlane, farPlane);
}