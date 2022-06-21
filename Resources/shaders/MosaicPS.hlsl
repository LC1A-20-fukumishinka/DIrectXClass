#include "Mosaic.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャー
SamplerState smp : register(s0); //0番スロットに設定されたテクスチャー

float4 main(VSOutput input) : SV_TARGET
{
    float Xpixel = 1.0f / 1280.0f;
    float Ypixel = 1.0f / 720.0f;
    
    int MosaicCount = 10;//定数バッファで指定できるように

    float XMosaicpixel = MosaicCount * Xpixel;
    float YMosaicpixel = MosaicCount * Ypixel;
    float4 texcolor = { 0, 0, 0, 0 };

    float XpixelCount = fmod(input.uv.x, XMosaicpixel);
    float YpixelCount = fmod(input.uv.y, YMosaicpixel);
    float BaseX = input.uv.x - XpixelCount;
    float BaseY = input.uv.y - YpixelCount;
    for (int y = 0; y < MosaicCount;y++)
    {
        for (int x = 0; x < MosaicCount;x++)
        {
            float Xpos = BaseX + (Xpixel * x);
            float Ypos = BaseY + (Ypixel * y);
            texcolor += tex.Sample(smp, float2(Xpos, Ypos));
        }
    }
    texcolor /= (MosaicCount * MosaicCount);
    //texcolor *= color;
    return float4(texcolor);
}