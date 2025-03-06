#pragma once
#include "Header\EngineCoreMinimal.h"

enum class E_RENDER_TARGET_FLAG : uint8
{
	DEFAULT = 0,

	END
};

class RenderTargetObject
{
public:

protected:

private:
	std::array<UINT, static_cast<uint8>(E_RENDER_TARGET_FLAG::END)> renderTargetNums{};
	std::array<std::array<DXGI_FORMAT, 8>, static_cast<uint8>(E_RENDER_TARGET_FLAG::END)> renderTargetViewFormats{};

public:
	bool Init();

	FORCEINLINE UINT GetRenderTargetNum(E_RENDER_TARGET_FLAG Flag)const { return renderTargetNums[static_cast<uint8>(Flag)]; }
	FORCEINLINE std::array<DXGI_FORMAT, 8> GetRenderTargetViewFormat(E_RENDER_TARGET_FLAG Flag)const { return renderTargetViewFormats[static_cast<uint8>(Flag)]; }

protected:

private:

};

