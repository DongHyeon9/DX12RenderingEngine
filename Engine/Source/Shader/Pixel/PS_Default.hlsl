#include "Default.hlsli"

float4 main(PixelShaderInput input) : SV_Target
{
    return albedoTex.Sample(warpSampler, input.texcoord);
}