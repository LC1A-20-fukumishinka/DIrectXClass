#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);
    VSOutput output; // ピクセルシェーダーに渡す値
    
    output.svpos = mul(mul(viewproj, world), pos);
    
    output.color.rgb = m_ambient + ambientColor;
    if (dirlight.active)
    {
        float3 diffuse = saturate(dot(dirlight.lightv, wnormal.xyz)) * m_diffuse;
        output.color.rgb = (((output.color.rgb + (diffuse * m_diffuse))) * dirlight.lightcolor);
    }
    output.color.a = m_alpha;
    output.uv = uv;
    return output;
}