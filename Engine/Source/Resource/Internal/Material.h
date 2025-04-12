#pragma once
#include "Header\EngineCore.h"

class Texture;

class Material
{
public:

protected:

private:
	MaterialParam matParam{};
	std::array<std::shared_ptr<Texture>, ENUM_COUNT::TEXTURE_TYPE> textures{};

public:
	void SetAmbient(const Vector3& Ambient);
	void SetSpecular(const Vector3& Specular);
	void SetDiffuse(const Vector3& Diffuse);
	void SetTexture(E_TEXTURE_TYPE Index, std::shared_ptr<Texture> Texture);

	void Render();

protected:

private:

};

