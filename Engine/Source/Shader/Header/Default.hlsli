#define RT_ALBEDO t0
#define RT_NORMAL t1
#define RT_METALLIC t2
#define RT_ROUGHNESS t3
#define RT_AMBIENT t4
#define RT_HEIGHT t5

#define TYPE_POINT 0
#define TYPE_SPOT 1
#define TYPE_DIR 2

#define USE_ALBEDO (1<<0)
#define USE_NORMAL (1<<1)
#define USE_METALLIC (1<<2)
#define USE_ROUGHNESS (1<<3)
#define USE_AMBIENT (1<<4)
#define USE_HEIGHT (1<<5)

#define RB_CAMERA b0
#define RB_LIGHT b1
#define RB_NORMAL b2
#define RB_TRANSFORM b3
#define RB_MATERIAL b4

#define MAX_LIGHT 50

struct VertexShaderInput
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct PixelShaderInput
{
    float4 projection : SV_POSITION;
    float4 world : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    float2 texcoord : TEXCOORD;
};

Texture2D albedoTex : register(RT_ALBEDO);
Texture2D normalTex : register(RT_NORMAL);
Texture2D metallicTex : register(RT_METALLIC);
Texture2D roughnessTex : register(RT_ROUGHNESS);
Texture2D ambientTex : register(RT_AMBIENT);
Texture2D heightTex : register(RT_HEIGHT);

SamplerState warpSampler : register(s0);
SamplerState clampSampler : register(s1);

struct LightInfo
{
    float3 color;
    float falloffStart;
    float3 direction;
    float falloffEnd;
    float3 position;
    float spotPower;
    uint lightType;
    float3 dummy;
};

cbuffer CameraData : register(RB_CAMERA)
{
    float4 cameraPos;
    row_major matrix view;
    row_major matrix projection;
};

cbuffer LightData : register(RB_LIGHT)
{
    uint lightCount;
    float3 dummy;
    LightInfo lightInfo[MAX_LIGHT];
};
 
cbuffer TransformMatrix : register(RB_TRANSFORM)
{
    row_major matrix worldMat;
    row_major matrix inverseWorldMat;
};

cbuffer NormalVector : register(RB_NORMAL)
{
    float scale;
};

cbuffer MaterialParams : register(RB_MATERIAL)
{
    float3 ambient;
    float shininess;
    float3 diffuse;
    uint textureInfo;
    float3 specular;
    float dummy2; // 16 bytes 맞춰주기 위해 추가
};