#include "BoxShader.hlsli"

//inputLayout
//{
//  float4 pos : POSITION ���W
//}
VSOutput main(float4 pos : POSITION)
{
	VSOutput output; // ���_�V�F�[�_�[����̃A�E�g�v�b�g���e
	output.pos = pos;
	return output;
}