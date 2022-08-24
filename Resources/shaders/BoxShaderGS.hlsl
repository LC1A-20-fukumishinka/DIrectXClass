#include "BoxShader.hlsli"

//�����̂̒��_��
static const uint sVNum = 4;

static const uint sFaceNum = 6;
//�Z���^�[����̃I�t�Z�b�g
static const float4 offset_array[sVNum] =
{
	float4(-0.5f, -0.5f, -0.5f, 0),	//������O
	float4(-0.5f, +0.5f, -0.5f, 0),	//�����O
	float4(+0.5f, -0.5f, -0.5f, 0),	//�E����O
	float4(+0.5f, +0.5f, -0.5f, 0),	//�E���O
};

static const float2 uv_array[sVNum] =
{
	float2(0, 1),
	float2(0, 0),
	float2(1, 1),
	float2(1, 0),
};

static const matrix rotation_array[sFaceNum] = 
{
	//����
	matrix(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	),
	//�E��
	matrix(
		0, 0, -1, 0,
		0, 1, 0, 0,
		1, 0, 0, 0,
		0, 0, 0, 1
	),
	//����
	matrix(
		-1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1
	),
	//����
	matrix(
		0, 0, 1, 0,
		0, 1, 0, 0,
		-1, 0, 0, 0,
		0, 0, 0, 1
	),
	//���
	matrix(
		1, 0, 0, 0,
		0, 0, -1, 0,
		0, 1, 0, 0,
		0, 0, 0, 1
	),
	//����
	matrix(
		1, 0, 0, 0,
		0, 0, 1, 0,
		0, -1, 0, 0,
		0, 0, 0, 1
	),
};
//�_�̓��͂���A�ʂ�8�o�͂���T���v��
[maxvertexcount(24)]//4���_ * 6�� = 24
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	//4�_����

	for(uint face = 0; face < sFaceNum;face++)
	{
		for (uint v = 0; v < sVNum; v++)
		{
			float4 offset;
			//float4 offset = mul(matBillboard, offset_array[v]);
			//���S����̃I�t�Z�b�g���X�P�[�����O
			offset = offset_array[v];

			//���ʂɍ��킹�ĕ����̏C��
			offset = mul(rotation_array[face], offset);

			//���[���h��]
			offset = mul(mat, offset);

			//���[���h���W�x�[�X�ŁA���炷
			element.svpos = input[0].pos + offset;

			//�r���[�A�ˉe�ϊ�
			element.svpos = mul(mat, element.svpos);
			element.uv = uv_array[v];
			output.Append(element);
		}
		//�e��`�ʐ������Ƃ�
		output.RestartStrip();
	}
}