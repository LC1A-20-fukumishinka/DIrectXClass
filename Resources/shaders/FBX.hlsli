cbuffer cbuff0 : register(b0)
{
    matrix viewproj;
    matrix world;
    float3 cameraPos;
}

static const int MAX_BONES = 32;

cbuffer skinning:register(b3)
{
    matrix matSkinning[MAX_BONES];
}

struct VSInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint4 boneIndices : BONEINDICES;    //ボーンの番号
    float4 boneWeights : BONEWEIGHTS;   //ボーンのスキンウェイト
};

struct VSOutput
{
    float4 svpos : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct PSOutpot
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};