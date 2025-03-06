#pragma once
#include "ShaderObjectBase.h"

enum class E_VERTEX_SHADER_FLAG : uint8
{
	DEFAULT = 0,

	END
};

class VertexShaderObject final : public INHERITANCE_SOB(E_VERTEX_SHADER_FLAG)
{
public:

protected:

private:
	std::array<std::vector<D3D12_INPUT_ELEMENT_DESC>, SHADER_COUNT> inputLayouts{};

public:
	VertexShaderObject() :ShaderObjectBase(EngineUtil::Path::GetVertexShaderDir(), TEXT("vs_5_0")) {}
	bool Init()override;
	D3D12_INPUT_LAYOUT_DESC GetInputLayout(E_VERTEX_SHADER_FLAG ShaderFlag)const;

protected:

private:
	void SetInputLayout(E_VERTEX_SHADER_FLAG ShaderFlag, std::vector<D3D12_INPUT_ELEMENT_DESC> InputLayout);

};

