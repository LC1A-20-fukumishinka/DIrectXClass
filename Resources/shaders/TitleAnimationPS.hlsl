#include "TitleAnimation.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャー
SamplerState smp : register(s0); //0番スロットに設定されたテクスチャー

float4 main(VSOutput input) : SV_TARGET
{
    
    float4 baseColor = tex.Sample(smp, input.uv) * color;
    float4 texcolor = baseColor;

    float rate = saturate(alpha);

    //じわじわ明転
    if(typeName == 0)
    {
    rate *= 2.0;
    rate -= 0.5;
    rate = saturate(rate);

    float3 animationRate = (rate,rate,rate);
    texcolor.rgb *= animationRate;
    }

    //下から上に
    if(typeName == 1)
    {
        //↑ = 0.0, ↓ = 1.0
        rate *= 3.0;
        rate -= 1.0;
        float UpperRate = (input.uv.y); 
        rate = UpperRate + rate;
        rate = saturate(rate);

        float3 animationRate = (rate,rate,rate);
        texcolor.rgb *= animationRate; 
    }

    if(typeName == 2)
    {
        //↑ = 0.0, ↓ = 1.0
        rate *= 3.5;
        rate -= 2.0;
        float2 center = (0.5, 0.5);
        float2 drawUv = (input.uv) - center;
        float alphaRate = 2 - abs(drawUv.x * 2) - abs(drawUv.y * 2);
        rate = alphaRate + rate;
        rate = saturate(rate);

        float3 animationRate = (rate,rate,rate);
        texcolor.rgb *= animationRate; 
    }
    return float4(texcolor);
}