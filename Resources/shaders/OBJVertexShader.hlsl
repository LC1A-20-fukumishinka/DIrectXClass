#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);
    VSOutput output; // ピクセルシェーダーに渡す値
    
    output.svpos = mul(mul(viewproj, world), pos);
    
    output.color.rgb = m_ambient + ambientColor;
    if (dirlight.active)
    {
        float3 diffuse = saturate(dot(dirlight.lightv, wnormal.xyz)) * m_diffuse;
        output.color.rgb = (((output.color.rgb + (diffuse * m_diffuse))) * dirlight.lightcolor);
    }
    if (pointlight.active)
    {
        //ライトへのベクトル
        float3 lightv = pointlight.lightpos - wpos.xyz;
        //ベクトルの長さ
        float d = length(lightv);
        //正規化し。単位ベクトルにする
        lightv = normalize(lightv);
        //距離減衰係数
        float atten = 1.0f / ((pointlight.lightatten.x) + (pointlight.lightatten.y * d) + (pointlight.lightatten.z * d * d));
        //ライトに向かうベクトルと法線の内積
        float3 dotlightnormal = dot(lightv, wnormal.xyz);
        //拡散反射光
        float3 diffuse = dotlightnormal * m_diffuse;
        //全て加算する
        output.color.rgb += atten * (diffuse) * pointlight.lightcolor;
    }
    output.color.a = m_alpha;
    output.uv = uv;
    return output;
}