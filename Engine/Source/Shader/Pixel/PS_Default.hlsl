#include "..\Header\Default.hlsli"

float4 main(PixelShaderInput input) : SV_Target
{
    //return albedoTex.Sample(warpSampler, input.texcoord);
    return float4(input.texcoord.r, input.texcoord.g, 0.0, 1.0);
}