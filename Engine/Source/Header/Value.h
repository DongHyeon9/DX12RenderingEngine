#pragma once
#include "Enum.h"

namespace RESOLUTION
{
	constexpr uint32 WIDTH{ 1280 };
	constexpr uint32 HEIGHT{ 720 };
}

namespace KEY
{
	constexpr uint16 TYPE_COUNT{ static_cast<uint16>(UINT8_MAX + 1) };
	constexpr uint8 STATE_COUNT{ static_cast<uint8>(E_KEY_STATE::END) };
}

namespace GLOBAL 
{
	constexpr float UNIT{ 1000.0f };

	const Color CLEAR_COLOR{ DirectX::Colors::AliceBlue };
	constexpr Vector3 FORWARD{ 0.f,0.f,1.f };
	constexpr Vector3 BACKWARD{ 0.f,0.f,-1.f };
	constexpr Vector3 RIGHT{ 1.f,0.f,0.f };
	constexpr Vector3 LEFT{ -1.f,0.f,0.f };
	constexpr Vector3 UP{ 0.f,1.f,0.f };
	constexpr Vector3 DOWN{ 0.f,-1.f,0.f };
}

namespace ENUM_COUNT
{
	constexpr uint8 CBV_REGISTER{ static_cast<uint8>(E_CBV_REGISTER::END) };
	constexpr uint8 SRV_REGISTER{ static_cast<uint8>(E_SRV_REGISTER::END) - CBV_REGISTER };
	constexpr uint8 CONSTANT_BUFFER_TYPE{ static_cast<uint8>(E_CONSTANT_BUFFER_TYPE::END) };
}

namespace LITERAL
{
	constexpr uint32 SWAP_CHAIN_BUFFER_COUNT{ 2 };
	constexpr uint16 REGISTER_COUNT{ ENUM_COUNT::CBV_REGISTER + ENUM_COUNT::SRV_REGISTER };
	constexpr uint8 GLOBAL_CBUFFER_COUNT{ 3 };
	constexpr uint8 MAX_LIGHT{ 50 };

	constexpr DXGI_FORMAT BACK_BUFFER_FORMAT{ DXGI_FORMAT_R8G8B8A8_UNORM };
	constexpr DXGI_FORMAT DEPTH_STENCIL_FORMAT{ DXGI_FORMAT_D24_UNORM_S8_UINT };
}