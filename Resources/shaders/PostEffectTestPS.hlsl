#include "PostEffectTest.hlsli"

Texture2D<float4> tex0 : register(t0); //0番スロットに設定されたテクスチャー
Texture2D<float4> tex1 : register(t1); //1番スロットに設定されたテクスチャー
SamplerState smp : register(s0); //0番スロットに設定されたテクスチャー

float4 main(VSOutput input) : SV_TARGET
{
    //float Xpixel = 1.0f / 1280.0f;
    //float Ypixel = 1.0f / 720.0f;
    //float4 texcolor = { 0, 0, 0, 0 };
    //for (int y = 0; y < 3;y++)
    //{
    //    for (int x = 0; x < 3; x++)
    //    {
    //        texcolor += tex.Sample(smp, input.uv + float2(-Xpixel * (x - 1), -Ypixel * (y - 1))) * color;
    //    }
    //}
    //texcolor /= 9;
    float4 texcolor0 = tex0.Sample(smp, input.uv) * color;
    float4 texcolor1 = tex1.Sample(smp, input.uv) * color;
    
    float4 drawColor = texcolor0;
    if(fmod(input.uv.y, 0.1f) < 0.05f)
    {
        drawColor = texcolor1;
    }
    return float4(drawColor.rgb, 1);
}