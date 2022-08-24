cbuffer cbuff0 : register(b0)
{
	matrix mat;//ワールド行列(回転とスケーリングのみ)
    matrix matViewprj;//ビュープロジェクション行列
	float4 color;//描画色
};

// 頂点シェーダー空のアウトプット構造体
struct VSOutput
{
	float4 pos : POSITION; // 中心座標
};

//ジオメトリシェーダーからのアウトプット構造体
struct GSOutput
{
	float4 svpos : SV_POSITION;	//座標
	float2 uv : TEXCOORD;
};