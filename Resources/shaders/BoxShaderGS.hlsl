#include "BoxShader.hlsli"

//立方体の頂点数
static const uint sVNum = 4;

static const uint sFaceNum = 6;
//センターからのオフセット
static const float4 offset_array[sVNum] =
{
	float4(-0.5f, -0.5f, -0.5f, 0),	//左下手前
	float4(-0.5f, +0.5f, -0.5f, 0),	//左上手前
	float4(+0.5f, -0.5f, -0.5f, 0),	//右下手前
	float4(+0.5f, +0.5f, -0.5f, 0),	//右上手前
};

static const float2 uv_array[sVNum] =
{
	float2(0, 1),
	float2(0, 0),
	float2(1, 1),
	float2(1, 0),
};

static const matrix rotation_array[sFaceNum] = 
{
	//正面
	matrix(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	),
	//右面
	matrix(
		0, 0, -1, 0,
		0, 1, 0, 0,
		1, 0, 0, 0,
		0, 0, 0, 1
	),
	//奥面
	matrix(
		-1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1
	),
	//左面
	matrix(
		0, 0, 1, 0,
		0, 1, 0, 0,
		-1, 0, 0, 0,
		0, 0, 0, 1
	),
	//上面
	matrix(
		1, 0, 0, 0,
		0, 0, -1, 0,
		0, 1, 0, 0,
		0, 0, 0, 1
	),
	//下面
	matrix(
		1, 0, 0, 0,
		0, 0, 1, 0,
		0, -1, 0, 0,
		0, 0, 0, 1
	),
};
//点の入力から、面を8つ出力するサンプル
[maxvertexcount(24)]//4頂点 * 6面 = 24
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	//4点分回す

	for(uint face = 0; face < sFaceNum;face++)
	{
		for (uint v = 0; v < sVNum; v++)
		{
			float4 offset;
			//float4 offset = mul(matBillboard, offset_array[v]);
			//中心からのオフセットをスケーリング
			offset = offset_array[v];

			//作る面に合わせて方向の修正
			offset = mul(rotation_array[face], offset);

			//ワールド回転
			offset = mul(mat, offset);

			//ワールド座標ベースで、ずらす
			element.svpos = input[0].pos + offset;

			//ビュー、射影変換
			element.svpos = mul(mat, element.svpos);
			element.uv = uv_array[v];
			output.Append(element);
		}
		//各矩形面生成ごとに
		output.RestartStrip();
	}
}