#pragma once
#include "Header.h"
#include "Value.h"

class AppBase;

struct EngineDesc
{
	std::shared_ptr<AppBase> app{};
	HINSTANCE hInstance{};
	uint32 width{ RESOLUTION::WIDTH };
	uint32 height{ RESOLUTION::HEIGHT };
};

struct EngineSetting
{
	HINSTANCE hInstance{};
	HWND hWnd{};
	Vector2 resolution{};
	FString engineName{ TEXT("DirectX 12 Renderer") };

	uint32 quality4XMSAA{};
	bool state4XMSAA{};
};

struct Transform
{
	Vector3 position{};
	Vector3 rotation{};
	Vector3 scale{ 1.0f };
};

struct CameraData
{
	Vector4 cameraPos{};
	Matrix view{ Matrix::Identity };
	Matrix projection{ Matrix::Identity };
};

struct LightInfo
{
	Vector3 color{ 1.0f, 1.0f, 1.0f };
	float falloffStart{};
	Vector3 direction{ 0.0f, 0.0f, 1.0f };
	float falloffEnd{ 10.0f };
	Vector3 position{};
	float spotPower{ 1.0f };
	E_LIGHT_TYPE lightType{};
	Vector3 dummy1{};
};

struct LightData
{
	uint32 lightCount{};
	Vector3 dummy{};
	std::array<LightInfo, LITERAL::MAX_LIGHT> lightInfo{};
};

struct NormalVectorData
{
	float scale{ 0.1f };
	float dummy[3];
};

struct MaterialParam
{
	Vector3 ambient{ 0.1f };
	float shininess{ 1.0f };
	Vector3 diffuse{ 1.0f };
	uint32 textureInfo{};
	Vector3 specular{ 0.5f };
	float dummy2{};
};

struct TransformMatrix
{
	Matrix worldMat{ Matrix::Identity };
	Matrix inverseWorldMat{ Matrix::Identity };
};

struct Vertex
{
	Vector3 position{};
	Vector3 normal{};
	Vector3 tagent{};
	Vector2 texcoord{};
};

struct MeshData
{
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
};

