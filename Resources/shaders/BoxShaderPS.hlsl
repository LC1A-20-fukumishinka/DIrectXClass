#include "BoxShader.hlsli"

Texture2D<float4> tex : register(t0);	// �g�p����e�N�X�`���[
SamplerState smp : register(s0);		// 0�Ԃ̃T���v���[

float4 main(GSOutput input) : SV_TARGET
{
	//return float4(tex.Sample(smp, input.uv) * color);
	return float4(color);
}