#include "BoxShader.hlsli"

Texture2D<float4> tex : register(t0);	// 使用するテクスチャー
SamplerState smp : register(s0);		// 0番のサンプラー

float4 main(GSOutput input) : SV_TARGET
{
	//return float4(tex.Sample(smp, input.uv) * color);
	return float4(color);
}