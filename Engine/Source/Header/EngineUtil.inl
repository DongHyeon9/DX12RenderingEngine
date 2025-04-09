#include "EngineUtil.h"
#pragma once

#pragma region Shader

inline void EngineUtil::Shader::CheckBlob(Microsoft::WRL::ComPtr<ID3DBlob> Error)
{
	if (Error)
	{
		const char* errorMessage = static_cast<const char*>(Error->GetBufferPointer());

#ifdef UNICODE
		FString errorStr = std::wstring(errorMessage, errorMessage + strlen(errorMessage));
#else
		FString errorStr = std::string(errorMessage);
#endif
		LOG("블롭 에러 : %s", errorStr.c_str());
	}
	else
	{
		LOG("알 수 없는 블롭 에러");
	}
}

inline bool EngineUtil::Shader::CreateShader(const FString& Path, const FString& EntryPoint, const FString& Version, Microsoft::WRL::ComPtr<ID3DBlob>& Blob, D3D12_SHADER_BYTECODE& ShaderByteCode)
{
	Blob.Reset();
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
	uint32 compileFlag = 0;
#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG;
#endif

	Microsoft::WRL::ComPtr<ID3DBlob> errBlob{};

	HRESULT hr{};
	hr = D3DCompileFromFile(
		String::ConvertString<std::wstring>(Path).c_str(),
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		String::ConvertString<std::string>(EntryPoint).c_str(),
		String::ConvertString<std::string>(Version).c_str(),
		flags, 0, Blob.GetAddressOf(), errBlob.GetAddressOf());

	if (FAILED(hr))
	{
		CheckBlob(errBlob);
		LOG("쉐이더 생성 실패 : %s", Path.c_str());
		return false;
	}

	ShaderByteCode = D3D12_SHADER_BYTECODE{ Blob->GetBufferPointer(), Blob->GetBufferSize() };
	return true;
}

inline UINT EngineUtil::Shader::CalulateConstantBufferSize(UINT ByteSize)
{
	// 상수 버퍼는 최소 하드웨어할당 크기(일반적으로 256바이트)의 배수여야 됨
	// 따라서 가장 가까운 256의 배수로 반올림
	// 255를 더한 다음 256보다 작은 모든 비트를 저장하는 하위 2바이트를 마스크하여 이를 수행
	// ex) byteSize = 300이라고 가정
	// (300 + 255) & ~255
	// 555 & ~255
	// 0x022B & ~0x00ff
	// 0x022B & 0xff00
	// 0x0200
	// 512
	return (ByteSize + 255) & ~255;
}

#pragma endregion Shader

#pragma region Path

template<class _Dst, class _Src>
inline _Dst EngineUtil::Path::AddDir(const _Dst& MainPath, const _Src& SubPath)
{
	_Dst result = MainPath;
	if (result.back() != TEXT('\\'))
		result += TEXT('\\');
	result += SubPath;
	result += TEXT('\\');
	return result;
}

template<class _Dst, class _Src, class ...Types>
inline _Dst EngineUtil::Path::AddDir(const _Dst& MainPath, const _Src& SubPath, Types ...Args)
{
	_Dst result = MainPath;
	if (result.back() != TEXT('\\'))
		result += TEXT('\\');
	result += SubPath;
	return AddDir(result, Args...);
}

inline FString EngineUtil::Path::PrevDir(const FString& Path, size_t Count)
{
	FString result{ Path };

	if (!result.empty() && result.back() == '\\')
		result.pop_back();

	for (size_t i = 0; i < Count; ++i)
	{
		while (!result.empty() && result.back() != '\\')
			result.pop_back();
	}

	return result;
}

inline FString EngineUtil::Path::GetCurrentDir()
{
#ifdef UNICODE
	FString result{ std::filesystem::current_path().c_str() };
#else
	FString result{ std::filesystem::current_path().string() };
#endif
	result += (TEXT('\\'));
	return result;
}

inline FString EngineUtil::Path::GetEngineDir()
{
	return AddDir(PrevDir(GetCurrentDir()), TEXT("Engine"));
}

inline FString EngineUtil::Path::GetTexturesDir()
{
	return AddDir(GetResourceDir(), TEXT("Texture"));
}

inline FString EngineUtil::Path::GetMeshDir()
{
	return AddDir(GetResourceDir(), TEXT("Mesh"));
}

inline FString EngineUtil::Path::GetResourceDir()
{
	return AddDir(PrevDir(GetCurrentDir()), TEXT("Res"));
}

inline FString EngineUtil::Path::GetShaderDir()
{
	return AddDir(GetEngineDir(), TEXT("Source"), TEXT("Shader"));
}

inline FString EngineUtil::Path::GetVertexShaderDir()
{
	return AddDir(GetShaderDir(), TEXT("Vertex"));
}

inline FString EngineUtil::Path::GetGeometryShaderDir()
{
	return AddDir(GetShaderDir(), TEXT("Geometry"));
}

inline FString EngineUtil::Path::GetDomainShaderDir()
{
	return AddDir(GetShaderDir(), TEXT("Tessellation"), TEXT("Domain"));
}

inline FString EngineUtil::Path::GetHullShaderDir()
{
	return AddDir(GetShaderDir(), TEXT("Tessellation"), TEXT("Hull"));
}

inline FString EngineUtil::Path::GetPixelShaderDir()
{
	return AddDir(GetShaderDir(), TEXT("Pixel"));
}

#pragma endregion Path

#pragma region String

template<class _Dst, class _Src>
inline _Dst EngineUtil::String::ConvertString(const _Src& String)
{
	if constexpr (std::is_same_v<_Dst, _Src>)
		return String;

	_Dst result{};
	for (auto c : String)
	{
		result += (TYPE(result))c;
	}
	return result;
}

#pragma endregion String

#pragma region Math

inline Vector3 EngineUtil::Math::ToEulerAngles(Quaternion Quaternion)
{
	Vector3 angles{};

	// roll (X축 회전)
	const double sinr_cosp = 2 * (Quaternion.w * Quaternion.x + Quaternion.y * Quaternion.z);
	const double cosr_cosp = 1 - 2 * (Quaternion.x * Quaternion.x + Quaternion.y * Quaternion.y);
	angles.x = static_cast<float>(std::atan2(sinr_cosp, cosr_cosp));

	// pitch (Y축 회전)
	const double sinp = std::sqrt(1 + 2 * (Quaternion.w * Quaternion.y - Quaternion.x * Quaternion.z));
	const double cosp = std::sqrt(1 - 2 * (Quaternion.w * Quaternion.y - Quaternion.x * Quaternion.z));
	angles.y = static_cast<float>(2 * std::atan2(sinp, cosp) - 3.14159f / 2);

	// yaw (Z축 회전)
	const double siny_cosp = 2 * (Quaternion.w * Quaternion.z + Quaternion.x * Quaternion.y);
	const double cosy_cosp = 1 - 2 * (Quaternion.y * Quaternion.y + Quaternion.z * Quaternion.z);
	angles.z = static_cast<float>(std::atan2(siny_cosp, cosy_cosp));

	return angles;
}

#pragma endregion Math