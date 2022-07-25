#include "FukuMath.h"

using namespace DirectX;
float FukuMath::CosineTheorem(const float a, const float b, const float c)
{
	float bunbo = (a*b*2);
	float bunshi = ((a*a) + (b*b) - (c*c));
	return (bunshi / bunbo);
}

DirectX::XMMATRIX FukuMath::GetMatRot(DirectX::XMVECTOR &v1, DirectX::XMVECTOR &v2, DirectX::XMVECTOR &cross)
{
	XMVECTOR upVector = XMVECTOR{ 0, 1, 0, 0 };

	//(����) �����
	if (!XMVector3Equal(v2, XMVectorZero()) && !XMVector3IsInfinite(v2))
	{
		upVector = v2;
	}

	//�J����Z��
	XMVECTOR frontAxisZ = v1;


	//���ʃx�N�g����0�������ꍇ
	bool isZero = XMVector3Equal(frontAxisZ, XMVectorZero());
	//���ʃx�N�g����������������
	bool isInfinite = XMVector3IsInfinite(frontAxisZ);
	bool isException = isZero && isInfinite;
	//0�x�N�g�����ƌ�����܂�Ȃ��̂ŏ��O
	if (isException)
	{
		frontAxisZ = {0.0f,0.0f ,1.0f , 0.0f };
	}


	//��x�N�g���Ɛ��ʃx�N�g�������ꂾ�������x�N�g���̕�����ύX
	if (XMVector3Equal(upVector, frontAxisZ))
	{//��x�N�g����ύX����
		XMVECTOR upVector = XMVECTOR{ 0, 0, -1, 0 };
	}

	//�x�N�g���̐��K��
	frontAxisZ = XMVector3Normalize(frontAxisZ);

	//�J������X��
	XMVECTOR rightAxisX;
	//X���͏������Z���̊O�ςŋ��܂�
	rightAxisX = XMVector3Cross(upVector, frontAxisZ);
	//�x�N�g���𐳋K��
	rightAxisX = XMVector3Normalize(rightAxisX);
	//�E�����x�N�g��
	cross = rightAxisX;
	//�J������Y��
	XMVECTOR upAxisY;
	//Y����Z����X���̊O�ςŋ��܂�
	upAxisY = XMVector3Cross(frontAxisZ, rightAxisX);
	//�x�N�g���̐��K��
	upAxisY = XMVector3Normalize(upAxisY);
	//������x�N�g��������������
	v2 = upAxisY;
	//�J������]�s��
	XMMATRIX matRot;
	//�J�������W�n�����[���h���W�n�̕ϊ��s��
	matRot.r[0] = rightAxisX;
	matRot.r[1] = upAxisY;
	matRot.r[2] = frontAxisZ;
	matRot.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	//���[�J���ϐ��Ԃ��Ă�
	return matRot;
}
