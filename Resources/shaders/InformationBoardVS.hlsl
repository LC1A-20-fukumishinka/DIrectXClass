#include "InformationBoard.hlsli"
VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);
    VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
    output.worldpos = wpos;
    output.normal = wnormal.xyz;
    output.svpos = mul(mul(viewproj, world), pos);
    output.uv = uv;

    return output;
}