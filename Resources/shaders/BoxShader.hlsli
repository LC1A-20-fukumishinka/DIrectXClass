cbuffer cbuff0 : register(b0)
{
	matrix mat;//���[���h�s��(��]�ƃX�P�[�����O�̂�)
    matrix matViewprj;//�r���[�v���W�F�N�V�����s��
	float4 color;//�`��F
};

// ���_�V�F�[�_�[��̃A�E�g�v�b�g�\����
struct VSOutput
{
	float4 pos : POSITION; // ���S���W
};

//�W�I���g���V�F�[�_�[����̃A�E�g�v�b�g�\����
struct GSOutput
{
	float4 svpos : SV_POSITION;	//���W
	float2 uv : TEXCOORD;
};