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
	Vector3 cameraPos{};
	Matrix view{ Matrix::Identity };
	Matrix projection{ Matrix::Identity };
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

struct TransformMatrix
{
	Matrix matWVP{ Matrix::Identity };
};