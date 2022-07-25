#include "ShadowSH.hlsli"
Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    return float4(0.8f,0.8f,0.8f,1.0f);
}
