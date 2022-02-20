#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);
    VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
    
    output.svpos = mul(mul(viewproj, world), pos);
    
    output.color.rgb = m_ambient + ambientColor;
    if (dirlight.active)
    {
        float3 diffuse = saturate(dot(dirlight.lightv, wnormal.xyz)) * m_diffuse;
        output.color.rgb = (((output.color.rgb + (diffuse * m_diffuse))) * dirlight.lightcolor);
    }
    if (pointlight.active)
    {
        //���C�g�ւ̃x�N�g��
        float3 lightv = pointlight.lightpos - wpos.xyz;
        //�x�N�g���̒���
        float d = length(lightv);
        //���K�����B�P�ʃx�N�g���ɂ���
        lightv = normalize(lightv);
        //���������W��
        float atten = 1.0f / ((pointlight.lightatten.x) + (pointlight.lightatten.y * d) + (pointlight.lightatten.z * d * d));
        //���C�g�Ɍ������x�N�g���Ɩ@���̓���
        float3 dotlightnormal = dot(lightv, wnormal.xyz);
        //�g�U���ˌ�
        float3 diffuse = dotlightnormal * m_diffuse;
        //�S�ĉ��Z����
        output.color.rgb += atten * (diffuse) * pointlight.lightcolor;
    }
    output.color.a = m_alpha;
    output.uv = uv;
    return output;
}