#pragma once
#include "ShaderObjectBase.h"

enum class E_HULL_SHADER_FLAG : uint8
{
	DEFAULT = 0,

	END
};

class HullShaderObject final : public INHERITANCE_SOB(E_HULL_SHADER_FLAG)
{
public:

protected:

private:

public:
	HullShaderObject() : ShaderObjectBase(EngineUtil::Path::GetHullShaderDir(), TEXT("hs_5_0")) {}
	bool Init()override;

protected:

private:

};

