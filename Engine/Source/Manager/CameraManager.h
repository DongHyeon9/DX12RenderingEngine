#pragma once
#include "Header\EngineCore.h"

class CameraManager
{
	DECLARE_SINGLETON(CameraManager);
public:

protected:

private:
	CameraData cameraData{};

	Vector3 cameraPosition{ 0.f,0.f,0.f };
	Vector3 cameraRotation{ 0.f,0.f,0.f };

	Vector3 forward{};
	Vector3 right{};
	Vector3 up{};

	float FOV{ 50.f };
	float nearPlane{ 0.01f };
	float farPlane{ 100.0f };

	float cameraMoveSpeed{ 1.0f };
	float cameraTurnSpeed{ 3.0f };

	POINT currentMousePos{};

public:
	bool Init();
	void Update(float DeltaTime);
	void Render();

	void SetFieldOfView(float Value) { FOV = Value; }
	void SetNearPlane(float Value) { nearPlane = Value; }
	void SetFarPlane(float Value) { farPlane = Value; }
	void SetCameraMoveSpeed(float Value) { cameraMoveSpeed = Value; }
	void SetCameraTurnSpeed(float Value) { cameraTurnSpeed = Value; }
	void SetCameraLocation(const Vector3& NewLocation);
	void SetCameraRotation(const Vector3& NewRotation);

	FORCEINLINE float GetFieldOfView()const { return FOV; }
	FORCEINLINE float GetNearPlane()const { return nearPlane; }
	FORCEINLINE float GetFarPlane()const { return farPlane; }
	FORCEINLINE float GetCameraMoveSpeed()const { return cameraMoveSpeed; }
	FORCEINLINE float GetCameraTurnSpeed()const { return cameraTurnSpeed; }
	FORCEINLINE Vector3 GetCameraLocation()const { return cameraPosition; }
	FORCEINLINE Vector3 GetCameraRotation()const { return cameraRotation; }

protected:

private:
	void CameraMove(float DeltaTime);
	void UpdateTransform();

};

