#pragma once
#include "Header\EngineCoreMinimal.h"

enum class E_BLEND_STATE_FLAG : uint8
{
	DEFAULT = 0,
	MIRROR,

	END
};

class BlendStateObject
{
public:

protected:

private:
	std::array<D3D12_BLEND_DESC, static_cast<uint8>(E_BLEND_STATE_FLAG::END)> blendStates{};

public:
	bool Init();

	FORCEINLINE D3D12_BLEND_DESC GetBlendState(E_BLEND_STATE_FLAG Flag)const { return blendStates[static_cast<uint8>(Flag)]; }

protected:
	void SetBlendState(E_BLEND_STATE_FLAG BlendStateFlag, D3D12_BLEND_DESC Desc);

private:

};

