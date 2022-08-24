#include "BoxShader.hlsli"

//inputLayout
//{
//  float4 pos : POSITION 座標
//}
VSOutput main(float4 pos : POSITION)
{
	VSOutput output; // 頂点シェーダーからのアウトプット内容
	output.pos = pos;
	return output;
}