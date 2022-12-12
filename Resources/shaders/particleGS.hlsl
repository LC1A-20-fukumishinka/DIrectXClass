#include "particle.hlsli"

//必要な頂点数
static const uint vnum = 4;

//オフセット位置の配列
static const float4 offset_array[vnum] =
{
	float4(-0.5f, -0.5f, 0, 0),	//????
	float4(-0.5f, +0.5f, 0, 0),	//????
	float4(+0.5f, -0.5f, 0, 0),	//?E??
	float4(+0.5f, +0.5f, 0, 0),	//?E??
};

static const float2 uv_array[vnum] =
{
	float2(0, 1),
	float2(0, 0),
	float2(1, 1),
	float2(1, 0),
};
//点の入力から、面を2つ出力するサンプル
[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	//4点分回す
	for (uint i = 0; i < vnum; i++)
	{
		float4 offset;
		//float4 offset = mul(matBillboard, offset_array[i]);
		//オフセットの値にスケーリングを乗算
		offset = offset_array[i] * input[0].scale;
		//ビルボード行列を乗算
		offset = mul(matBillboard, offset);
		//頂点の位置に平行移動
		element.svpos = input[0].pos + offset;
		//ビュープロジェクション行列を乗算
		element.svpos = mul(mat, element.svpos);
		element.uv = uv_array[i];
		element.color = input[0].color;
		output.Append(element);
	}

}