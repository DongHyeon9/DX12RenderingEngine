#include "Material.h"

#include "Manager\RenderManager.h"
#include "Engine\Render\PipelineStateObject.h"
#include "Engine\Render\PipelineStateObject\RootSignatureObject.h"

void Material::SetRenderingFlag(E_RENDERING_FLAG Flag)
{
	renderingFlag = Flag;
}

void Material::SetAmbient(const Vector3& Ambient)
{
	matParam.ambient = Ambient;
}

void Material::SetSpecular(const Vector3& Specular)
{
	matParam.specular = Specular;
}

void Material::SetDiffuse(const Vector3& Diffuse)
{
	matParam.diffuse = Diffuse;
}

void Material::SetTexture(E_TEXTURE_TYPE Index, std::shared_ptr<Texture> Texture)
{
	textures[static_cast<uint8>(Index)] = Texture;
}

void Material::Render()
{

	std::shared_ptr<RootSignatureObject> rootSignature = PSO->GetRootSignature();
	rootSignature->PushData(E_CONSTANT_BUFFER_TYPE::MATERIAL, &matParam, sizeof(matParam));

	for (size_t i = 0; i < textures.size(); ++i)
	{
		if (textures[i] == nullptr)
			continue;

		matParam.textureInfo |= (1 << i);
		E_SRV_REGISTER reg{ static_cast<E_SRV_REGISTER>(static_cast<uint8>(E_SRV_REGISTER::T0) + i) };
		rootSignature->PushTexture(reg, textures[i]);
	}

	RenderManager::GetInstance()->SetPipelineState(renderingFlag);
}
