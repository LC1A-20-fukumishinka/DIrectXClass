cbuffer cbuff0 : register(b0)
{
    float4 color; //色(RGBA)
	//matrix mat; // ３Ｄ変換行列
    matrix viewproj;
    matrix world;
    float3 cameraPos;
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float3 normal : NORMAL; // 法線ベクトル
    float4 worldpos : POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD; // uv値
};
