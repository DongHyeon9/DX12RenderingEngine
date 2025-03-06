#pragma once
#include "ShaderObjectBase.h"

enum class E_GEOMETRY_SHADER_FLAG : uint8
{
	DEFAULT = 0,

	END
};

class GeometryShaderObject final : public INHERITANCE_SOB(E_GEOMETRY_SHADER_FLAG)
{
public:

protected:

private:

public:
	GeometryShaderObject() : ShaderObjectBase(EngineUtil::Path::GetGeometryShaderDir(), TEXT("gs_5_0")) {}
	bool Init()override;

protected:

private:

};

