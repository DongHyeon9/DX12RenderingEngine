#pragma once
#include "ShaderObjectBase.h"

enum class E_DOMAIN_SHADER_FLAG : uint8
{
	DEFAULT = 0,

	END
};

class DomainShaderObject final : public INHERITANCE_SOB(E_DOMAIN_SHADER_FLAG)
{
public:

protected:

private:

public:
	DomainShaderObject() : ShaderObjectBase(EngineUtil::Path::GetDomainShaderDir(), TEXT("ds_5_0")) {}
	bool Init()override;

protected:

private:

};

