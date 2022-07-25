
cbuffer cbuff0 : register(b0)
{
	float4 color;	//色
	matrix mat;
}

//頂点シェーダーから出ていく情報
struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};