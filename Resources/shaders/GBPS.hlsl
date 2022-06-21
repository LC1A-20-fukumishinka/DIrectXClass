#include "GB.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャー
SamplerState smp : register(s0); //0番スロットに設定されたテクスチャー

float4 main(VSOutput input) : SV_TARGET
{
    float4 baseColor = tex.Sample(smp, input.uv) * color;
    float3 Gray = { 0.2126f, 0.7152f, 0.0722f };
    float Y = dot(baseColor.rgb, Gray);
    float4 texcolor = { 0.0f, 0.0f, 0.0f, baseColor.a };

    //コンストバッファで設定できるように
    float LevelA = 0.7f;
    float LevelB = 0.35f;
    float LevelC = 0.125f;

    float4 colorA = { 0.612f, 0.725f, 0.086f, 1 };
    float4 colorB = { 0.549f, 0.667f, 0.078f, 1 };
    float4 colorC = { 0.188f, 0.392f, 0.188f, 1 };
    float4 colorD = { 0.063f, 0.247f, 0.063f, 1 };

    if (Y >= LevelA)
    {
        texcolor = colorA;
    }
    else if (Y >= LevelB)
    {
        texcolor = colorB;
    }
    else if (Y >= LevelC)
    {
        texcolor = colorC;
    }
    else
    {
        texcolor = colorD;
    }
    
    //float4 texcolor = { Y, Y, Y, baseColor.a };

    //float4 texcolor = tex.Sample(smp, input.uv) * color;

    return float4(texcolor);
}