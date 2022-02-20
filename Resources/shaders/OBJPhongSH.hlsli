cbuffer cbuff0 : register(b0)
{
    float4 color; //�F(RGBA)
	//matrix mat; // �R�c�ϊ��s��
    matrix viewproj;
    matrix world;
    float3 cameraPos;
};

cbuffer cbuff1 : register(b1)
{
    float3 m_ambient : packoffset(c0); //�A���r�G���g�W��
    float3 m_diffuse : packoffset(c1); //�f�B�t�[�Y�W��
    float3 m_specular : packoffset(c2); //�X�y�L�����[�W��
    float m_alpha : packoffset(c2.w); //�A���t�@
}
struct dirLight
{
    float3 lightv;
    float3 lightcolor;
    uint active;
};

struct PointLight
{
    float3 lightpos;
    float3 lightcolor;
    float3 lightatten;
    uint active;
};

cbuffer cbuff2 : register(b2)
{
    float3 ambientColor;
    dirLight dirlight;
    PointLight pointlight;
}

//cbuffer cbuff2 : register(b2)
//{
//    float3 lightv;
//    float3 lightcolor;
//    uint active;
//}

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float3 normal : NORMAL; // �@���x�N�g��
    float4 worldpos : POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD; // uv�l
};
