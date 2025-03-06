#pragma once
#include "Header\EngineCoreMinimal.h"

enum class E_RASTERIZER_STATE_FLAG : uint8
{
	SOLID = 0,
	WIRE_FRAME,
	SOLID_CCW,
	WIRE_FRAME_CCW,
	POST_PROCESSING,

	END
};

class RasterizerStateObject
{
public:

protected:

private:
	std::array<D3D12_RASTERIZER_DESC, static_cast<uint8>(E_RASTERIZER_STATE_FLAG::END)> rasterizeStates{};

public:
	bool Init();

	FORCEINLINE D3D12_RASTERIZER_DESC GetRasterizeState(E_RASTERIZER_STATE_FLAG Flag)const { return rasterizeStates[static_cast<uint8>(Flag)]; }

protected:
	void SetRasterizerState(E_RASTERIZER_STATE_FLAG RasterizeStateFlag, D3D12_RASTERIZER_DESC Desc);

private:

};

