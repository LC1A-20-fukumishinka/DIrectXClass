#include "OBJPhongSH.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);

    //シェーディングによる色
    float4 shadecolor;
    //光沢度
    const float shininess = 4.0f;
        //環境反射光
    float3 ambient = m_ambient + ambientColor;
    shadecolor.rgb = ambient;

    //頂点から視点への方向ベクトル
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

    //平行光源
    if (dirlight.active)
    {
    //ライトに向かうベクトルと法線の内積
        float3 dotlightnormal = dot(dirlight.lightv, input.normal);
    //反射光ベクトル
        float3 reflect = normalize(-dirlight.lightv + (2 * dotlightnormal * input.normal));

    //拡散反射光
        float3 diffuse = dotlightnormal * m_diffuse;
    //鏡面反射光
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
    //全て加算する
        shadecolor.rgb = (ambient + diffuse + specular) * dirlight.lightcolor;
    }
    
    //点光源
    if(pointlight.active)
    {
        //ライトへのベクトル
        float3 lightv = pointlight.lightpos - input.worldpos.xyz;
        //ベクトルの長さ
        float d = length(lightv);
        //正規化し、単位ベクトルにする
        lightv = normalize(lightv);
        //距離減衰係数
        float atten = 1.0f / ((pointlight.lightatten.x) + (pointlight.lightatten.y * d) + (pointlight.lightatten.z * d * d));
        //ライトに向かうベクトルと法線の内積
        float3 dotlightnormal = dot(lightv, input.normal);
        //反射光ベクトル
        float3 reflect = dotlightnormal * m_diffuse;
        //拡散反射光
        float3 diffuse = dotlightnormal * m_diffuse;
        //鏡面反射光
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
        //全て加算する
        shadecolor.rgb += atten * (diffuse + specular) * pointlight.lightcolor;

    }
    shadecolor.a = m_alpha;
    return float4(shadecolor * texcolor);
}
