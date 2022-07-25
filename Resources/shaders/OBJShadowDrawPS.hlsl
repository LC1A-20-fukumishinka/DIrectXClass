#include "OBJShadowDrawSH.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> shadowTex : register(t1);
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
        shadecolor.rgb += (ambient + diffuse + specular) * dirlight.lightcolor;
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
    
    if(spotlight.active)
    {
        float3 lightv = spotlight.lightpos - input.worldpos.xyz;
        float d = length(lightv);
        lightv = normalize(lightv);
        //距離減衰係数
        float atten = 1.0f / ((spotlight.lightatten.x) + (spotlight.lightatten.y * d) + (spotlight.lightatten.z * d * d));
        //角度減衰
        float cos = dot(lightv, spotlight.lightv);
        //減衰開始角度から、減衰終了角度にかけて減衰
        //減衰開始角度の内側の１倍 減衰終了角度の外側は０倍の輝度
        float angleatten = smoothstep(spotlight.lightfactoranglecos.y, spotlight.lightfactoranglecos.x, cos);
        //角度減衰を乗算
        atten *= angleatten;
        //ライトに向かうベクトルと法線の内積
        float3 dotlightnormal = dot(lightv, input.normal);
        //反射光ベクトル
        float3 reflect = normalize(-lightv + (2 * dotlightnormal * input.normal));
        //拡散反射光
        float3 diffuse = dotlightnormal * m_diffuse;
        //鏡面反射光
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
        //全て加算する
        shadecolor.rgb += atten * (diffuse + specular) * spotlight.lightcolor;
    }
    shadecolor.a = m_alpha;

    float2 shadowMapUV = input.posInLVP.xy / input.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;

    float3 shadowMap = 1.0f;
    if(shadowMapUV.x > 0.0f &&
     shadowMapUV.x < 1.0f && 
     shadowMapUV.y > 0.0f && 
     shadowMapUV.y < 1.0f)
     {
        shadowMap = shadowTex.Sample(smp, shadowMapUV);
        shadecolor.rgb -= shadowMap;
     }

    return float4(shadecolor * texcolor * color);
}