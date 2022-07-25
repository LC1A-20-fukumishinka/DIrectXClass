#include "BaseDraw.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;	//頂点シェーダーからピクセルシェーダーへ送られる構造体
	output.svpos = pos;	//座標
	output.uv = uv;//UV
	return output;
}

