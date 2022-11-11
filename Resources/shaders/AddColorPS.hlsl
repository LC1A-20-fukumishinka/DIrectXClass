#include "AddColor.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャー
Texture2D<float4> brightTex : register(t1);
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    //色を受け取って
    float4 baseColor = tex.Sample(smp, input.uv) * color;
    
    float4 brightColor = brightTex.Sample(smp, input.uv);

    brightColor = clamp(brightColor, 0.0f, 0.2f);
    baseColor += brightColor;
    
   return float4(baseColor);
}