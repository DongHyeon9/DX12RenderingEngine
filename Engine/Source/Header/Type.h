#pragma once
#include "DirectXMath.h"
#include "directxtk12/SimpleMath.h"
#include "directxtk12/SimpleMath.inl"
#include <string>

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using Vector2 = DirectX::SimpleMath::Vector2;
using Vector3 = DirectX::SimpleMath::Vector3;
using Vector4 = DirectX::SimpleMath::Vector4;
using Matrix = DirectX::SimpleMath::Matrix;
using Quaternion = DirectX::SimpleMath::Quaternion;
using Color = DirectX::XMVECTORF32;

#ifdef UNICODE
using FString = std::wstring;

#else
using FString = std::string;

#endif