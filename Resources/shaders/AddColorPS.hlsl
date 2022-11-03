#include "AddColor.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャー
Texture2D<float4> brightTex : register(t1);
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    //色を受け取って
    float4 baseColor = tex.Sample(smp, input.uv) * color;
    
    baseColor += brightTex.Sample(smp, input.uv);
   return float4(baseColor);
}