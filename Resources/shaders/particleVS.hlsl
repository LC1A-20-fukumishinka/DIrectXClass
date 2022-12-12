#include "particle.hlsli"

VSOutput main(float4 pos : POSITION, float4 color : COLOR, float scale : TEXCOORD)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	output.pos = pos;
	output.scale = scale;
	output.color = color;
	return output;
}