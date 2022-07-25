#include "BaseDraw.hlsli"

Texture2D<float4> tex : register(t0); //MainTex
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 baseColor = tex.Sample(smp, input.uv) * color;
    return baseColor;
}