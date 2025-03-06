#pragma once
#include "Shader\VertexShaderObject.h"
#include "Shader\HullShaderObject.h"
#include "Shader\DomainShaderObject.h"
#include "Shader\GeometryShaderObject.h"
#include "Shader\PixelShaderObject.h"

class ShaderObject
{
public:

protected:

private:
	std::unique_ptr<VertexShaderObject> vertexShaderObj{};
	std::unique_ptr<HullShaderObject> hullShaderObj{};
	std::unique_ptr<DomainShaderObject> domainShaderObj{};
	std::unique_ptr<GeometryShaderObject> geometryShaderObj{};
	std::unique_ptr<PixelShaderObject> pixelShaderObj{};

public:
	bool Init();

	FORCEINLINE D3D12_SHADER_BYTECODE GetVertexShader(E_VERTEX_SHADER_FLAG Flag)const { return vertexShaderObj->GetShader(Flag); }
	FORCEINLINE D3D12_SHADER_BYTECODE GetHullShader(E_HULL_SHADER_FLAG Flag)const { return hullShaderObj->GetShader(Flag); }
	FORCEINLINE D3D12_SHADER_BYTECODE GetDomainShader(E_DOMAIN_SHADER_FLAG Flag)const { return domainShaderObj->GetShader(Flag); }
	FORCEINLINE D3D12_SHADER_BYTECODE GetGeometryShader(E_GEOMETRY_SHADER_FLAG Flag)const { return geometryShaderObj->GetShader(Flag); }
	FORCEINLINE D3D12_SHADER_BYTECODE GetPixelShader(E_PIXEL_SHADER_FLAG Flag)const { return pixelShaderObj->GetShader(Flag); }

	FORCEINLINE D3D12_INPUT_LAYOUT_DESC GetInputLayoutDesc(E_VERTEX_SHADER_FLAG Flag)const { return vertexShaderObj->GetInputLayout(Flag); }

protected:

private:

};

