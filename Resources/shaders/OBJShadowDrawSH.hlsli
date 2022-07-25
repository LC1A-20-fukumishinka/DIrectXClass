cbuffer cbuff0 : register(b0)
{
    float4 color; //色(RGBA)
    matrix viewproj;
    matrix shadowViewProj;
    matrix world;
    float3 cameraPos;
};

cbuffer cbuff1 : register(b1)
{
    float3 m_ambient : packoffset(c0); //アンビエント係数
    float3 m_diffuse : packoffset(c1); //ディフーズ係数
    float3 m_specular : packoffset(c2); //スペキュラー係数
    float m_alpha : packoffset(c2.w); //アルファ
}
struct dirLight
{
    float3 lightv;
    float3 lightcolor;
    uint active;
};

struct PointLight
{
    float3 lightpos;
    float3 lightcolor;
    float3 lightatten;
    uint active;
};

struct SpotLight
{
    float3 lightv;
    float3 lightpos;
    float3 lightcolor;
    float3 lightatten;
    float2 lightfactoranglecos;
    uint active;
};

cbuffer cbuff2 : register(b2)
{
    float3 ambientColor;
    dirLight dirlight;
    PointLight pointlight;
    SpotLight spotlight;
}


// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float3 normal : NORMAL; // 法線ベクトル
    float4 worldpos : POSITION;
    float2 uv : TEXCOORD0; // uv値
    float4 posInLVP : TEXCOORD1; 
};
