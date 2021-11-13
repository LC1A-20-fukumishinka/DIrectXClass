#include "Camera.h"
using namespace DirectX;
Camera::Camera()
{
	this->eye = { 0.0f, 0.0f, -100.0f };
	this->target = { 0.0f, 0.0f, 0.0f };
	this->up = { 0.0f, 1.0f, 0.0f };
	position = { 0.0f, 0.0f, 0.0f };
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	matBillBoard = XMMatrixIdentity();
	matBillBoardY = XMMatrixIdentity();
}

void Camera::Init(const DirectX::XMFLOAT3 &eye, const DirectX::XMFLOAT3 &target, const DirectX::XMFLOAT3 &position, const DirectX::XMFLOAT3 &up)
{
	this->eye = eye;
	this->target = target;
	this->up = up;
	this->position = position;
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	matView *= XMMatrixTranslation(position.x, position.y, position.z);
}

void Camera::Update()
{
	//matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	//matView *= XMMatrixTranslation(position.x, position.y, position.z);
	MakeMatCamera();
}

DirectX::XMMATRIX Camera::GetMatBillboard() const
{
	return matBillBoard;
}

DirectX::XMMATRIX Camera::GetMatBillboardY() const
{
	return matBillBoardY;
}

void Camera::MakeMatCamera()
{
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	//�����_���W
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	//(����) �����
	XMVECTOR upVector = XMLoadFloat3(&up);

	//�J����Z��
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	//0�x�N�g�����ƌ�����܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	//�x�N�g���̐��K��
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//�J������X��
	XMVECTOR cameraAxisX;
	//X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	//�x�N�g���𐳋K��
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//�J������Y��
	XMVECTOR cameraAxisY;
	//Y����Z����X���̊O�ςŋ��܂�
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	//�x�N�g���̐��K��
	cameraAxisY = XMVector3Normalize(cameraAxisY);

	//�J������]�s��
	XMMATRIX matCameraRot;
	//�J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	//�]�u�ɂ��t�s��(�t��])���v�Z
	matView = XMMatrixTranspose(matCameraRot);

	//���_���W��-1���|�������W
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	//�J�����̈ʒu���烏�[���h���_�ւ̃x�N�g��(�J�������W�n)
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);
	//��̃x�N�g���ɊX����
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	//�r���[�s��ɕ��s�ړ�������ݒ�
	matView.r[3] = translation;

#pragma region �S�����r���{�[�h
	//�r���{�[�h
	matBillBoard.r[0] = cameraAxisX;
	matBillBoard.r[1] = cameraAxisY;
	matBillBoard.r[2] = cameraAxisZ;
	matBillBoard.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
#pragma endregion


#pragma region Y�����r���{�[�h�s��v�Z
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	//X���͋���
	ybillCameraAxisX = cameraAxisX;
	//Y���͘a0���h���W�n��Y��
	ybillCameraAxisY = XMVector3Normalize(upVector);
	//Z����X����Y���̊O�ςŋ��܂�
	ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	//Y������r���{�[�h�s��
	matBillBoardY.r[0] = ybillCameraAxisX;
	matBillBoardY.r[1] = ybillCameraAxisY;
	matBillBoardY.r[2] = ybillCameraAxisZ;
	matBillBoardY.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
#pragma endregion

}
