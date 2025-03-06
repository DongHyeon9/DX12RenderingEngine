#pragma once
#include "PreProcess.h"
#include "Struct.h"

namespace EngineUtil
{
	struct Shader
	{
		static void CheckBlob(Microsoft::WRL::ComPtr<ID3DBlob> Error);
		static bool CreateShader(const FString& Path, const FString& EntryPoint, const FString& Version, Microsoft::WRL::ComPtr<ID3DBlob>& Blob, D3D12_SHADER_BYTECODE& ShaderByteCode);
		static UINT CalulateConstantBufferSize(UINT ByteSize);
	};

	struct Path
	{
		template<class _Dst, class _Src>
		static _Dst AddDir(const _Dst& MainPath, const _Src& SubPath);

		template<class _Dst, class _Src, class... Types>
		static _Dst AddDir(const _Dst& MainPath, const _Src& SubPath, Types ...Args);

		static FString PrevDir(const FString& Path, size_t Count = 1);
		static FString GetCurrentDir();
		static FString GetEngineDir();
		static FString GetTexturesDir();
		static FString GetMeshDir();
		static FString GetResourceDir();
		static FString GetShaderDir();
		static FString GetVertexShaderDir();
		static FString GetGeometryShaderDir();
		static FString GetDomainShaderDir();
		static FString GetHullShaderDir();
		static FString GetPixelShaderDir();
	};

	struct String
	{
		template<class _Dst, class _Src>
		static _Dst ConvertString(const _Src& String);
	};

	struct Math
	{
		static Vector3 ToEulerAngles(Quaternion Quaternion);
	};

}

#include "EngineUtil.inl"