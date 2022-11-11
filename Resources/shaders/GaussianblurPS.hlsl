#include "Gaussianblur.hlsli"

Texture2D<float4> BrightTex : register(t0); //メインの描画テクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
    float4 brightColor = BrightTex.Sample(smp, input.uv);
        //    重みの総和       シグマ           段階
    float totalWeight = 0, _sigma = 0.1f, StepWidth = 0.02f;
    float4 blurColor = {0.0f, 0.0f, 0.0f, 0.0f};

    for(float py = -_sigma * 2; py <= _sigma * 2;py += StepWidth) 
    {
        for(float px = -_sigma * 2; px <= _sigma * 2; px += StepWidth)
        {
            float2 pickUV = input.uv + float2(px, py);
            float weight = Gaussian(input.uv, pickUV, _sigma);
            //重みづけした分だけ色を加算
            blurColor += (BrightTex.Sample(smp, pickUV) * weight); 
            //重みを加算していく
            totalWeight += weight;
        }
    }

    //重みの総和で除算
    blurColor.rgb = blurColor.rgb / totalWeight;
    blurColor.a = 1.0f;

   return float4(blurColor);
}

