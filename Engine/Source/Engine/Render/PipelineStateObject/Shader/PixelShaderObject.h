#pragma once
#include "ShaderObjectBase.h"

enum class E_PIXEL_SHADER_FLAG : uint8
{
	DEFAULT = 0,

	END
};

class PixelShaderObject final : public INHERITANCE_SOB(E_PIXEL_SHADER_FLAG)
{
public:

protected:

private:

public:
	PixelShaderObject() : ShaderObjectBase(EngineUtil::Path::GetPixelShaderDir(), TEXT("ps_5_0")) {}
	bool Init()override;

protected:

private:

};

