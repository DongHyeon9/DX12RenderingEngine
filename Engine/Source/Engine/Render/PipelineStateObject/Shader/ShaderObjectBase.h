#pragma once
#include "Header\EngineCoreMinimal.h"

#define INHERITANCE_SOB(Flag) ShaderObjectBase<Flag, static_cast<uint8>(Flag::END)>

template<class _ShaderFlag, uint8 _ShaderCount>
class ShaderObjectBase abstract
{
public:

protected:
	static constexpr uint8 SHADER_COUNT = _ShaderCount;
	std::array<D3D12_SHADER_BYTECODE, SHADER_COUNT> byteCodes{};
	std::array<Microsoft::WRL::ComPtr<ID3DBlob>, SHADER_COUNT> shaders{};

private:
	FString path{};
	FString version{};

public:
	ShaderObjectBase(const FString& Path, const FString& Version) : path(Path), version(Version) {}
	virtual bool Init() abstract;
	FORCEINLINE D3D12_SHADER_BYTECODE GetShader(_ShaderFlag ShaderFlag)const { return byteCodes[static_cast<uint8>(ShaderFlag)]; }

protected:
	bool CreateShader(const FString& Name, _ShaderFlag ShaderFlag)
	{
		size_t index = static_cast<size_t>(ShaderFlag);
		FString fullPath = path + Name + TEXT(".hlsl");
		if (EngineUtil::Shader::CreateShader(fullPath, TEXT("main"), version, shaders[index], byteCodes[index]))
		{
			LOG("%s 积己 己傍", Name.c_str());
			return true;
		}
		else
		{
			LOG("%s 积己 角菩", Name.c_str());
			return false;
		}
	}

private:

};
