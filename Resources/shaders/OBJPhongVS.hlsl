#include "OBJPhongSH.hlsli"
VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);
    VSOutput output; // ピクセルシェーダーに渡す値
    output.worldpos = wpos;
    output.normal = wnormal.xyz;
    output.svpos = mul(mul(viewproj, world), pos);
    output.uv = uv;
    //float3 eyedir = normalize(cameraPos - wpos.xyz);
    //float3 diffuse = saturate(dot(lightv, wnormal.xyz)) * m_diffuse;
    //output.color.rgb = (((m_ambient + (diffuse * m_diffuse))) * lightcolor);
    //output.color.a = m_alpha;
    return output;
}