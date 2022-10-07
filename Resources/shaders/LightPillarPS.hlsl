#include "LightPillar.hlsli"

//Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{

    input.uv.x -= 0.5f;

    float tmpy = 0.02f;
    input.uv.y = (1.0f - input.uv.y);
    input.uv.y -= tmpy;


    float4 texcolor = color;

    texcolor.a *= (0.5f - abs(input.uv.x));
    texcolor.a *= (1.0f - tmpy - abs(input.uv.y));

    if(input.uv.y < 0.0f)
    {
    texcolor.a *= (1.0f - ( abs(input.uv.y) / tmpy ));
    }
    //if(abs(input.uv.x) <=0.02f)
    //{
    //    texcolor = float4(1.0f,1.0f,1.0f, 1.0f);
    //}

    texcolor.a *= rate;
    return float4(texcolor);
}
