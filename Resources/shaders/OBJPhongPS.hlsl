#include "OBJPhongSH.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);

    //�V�F�[�f�B���O�ɂ��F
    float4 shadecolor;
    //����x
    const float shininess = 4.0f;
        //�����ˌ�
    float3 ambient = m_ambient + ambientColor;
    shadecolor.rgb = ambient;
    if (dirlight.active)
    {
    //���_����n�_�ւ̕����x�N�g��
        float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
    //���C�g�Ɍ������x�N�g���Ɩ@���̓���
        float3 dotlightnormal = dot(dirlight.lightv, input.normal);
    //���ˌ��x�N�g��
        float3 reflect = normalize(-dirlight.lightv + (2 * dotlightnormal * input.normal));

    //�g�U���ˌ�
        float3 diffuse = dotlightnormal * m_diffuse;
    //���ʔ��ˌ�
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
    //�S�ĉ��Z����
        shadecolor.rgb = (ambient + diffuse + specular) * dirlight.lightcolor;
    }
    shadecolor.a = m_alpha;
    return float4(shadecolor * texcolor);
}
