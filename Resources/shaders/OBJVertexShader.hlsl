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
    if (spotlight.active)
    {
        //ライトの方向ベクトル
        float3 lightv = spotlight.lightpos - wpos.xyz;
        float d = length(lightv);
        lightv = normalize(lightv);
        //距離減衰係数
        float atten = saturate(1.0f / (spotlight.lightatten.x + (spotlight.lightatten.y * d) + (spotlight.lightatten.z * d * d)));
        //角度減衰
        float cos = dot(lightv, spotlight.lightv);
        //減衰開始角度から、減衰終了角度にかけて減衰
        //減衰開始角度の内側の１倍 減衰終了角度の外側は０倍の輝度
        float angleatten = smoothstep(spotlight.lightfactoranglecos.y, spotlight.lightfactoranglecos.x, cos);
        //角度減衰を乗算
        atten *= angleatten;
        //ライトに向かうベクトルと法線の内積
        float3 dotlightnormal = dot(lightv, wnormal.xyz);
        //拡散反射光
        float3 diffuse = dotlightnormal * m_diffuse;
        
        //全てを加算する
        output.color.rgb += atten * diffuse * spotlight.lightcolor;
    }
    output.color.a = m_alpha;
    output.uv = uv;
    return output;
}