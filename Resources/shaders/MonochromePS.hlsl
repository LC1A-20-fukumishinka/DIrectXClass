#include "Monochrome.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`���[
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 baseColor = tex.Sample(smp, input.uv) * color;
    float3 Gray = { 0.2126f, 0.7152f, 0.0722f };
    float Y = dot(baseColor.rgb, Gray);

    float4 texcolor = { Y, Y, Y, baseColor.a };

    return float4(texcolor);
}