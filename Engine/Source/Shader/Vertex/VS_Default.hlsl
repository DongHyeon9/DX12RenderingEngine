#include "..\Header\Default.hlsli"

PixelShaderInput main(VertexShaderInput input)
{
    PixelShaderInput output;
    float4 worldPos = mul(float4(input.position, 1.0), worldMat);
    float4 prjPos = mul(worldPos, view);
    prjPos = mul(prjPos, projection);
    
    output.projection = prjPos;
    output.world = worldPos;
    output.texcoord = input.texcoord;
    
    output.normal = normalize(mul(float4(input.normal, 1.0), inverseWorldMat)).xyz;
    output.tangent = normalize(mul(float4(input.tangent, 1.0), inverseWorldMat)).xyz;
    output.biNormal = normalize(cross(output.normal, output.tangent));
    
    return output;
}