#include "Camera.h"
#include "MyDirectX.h"
#include "FukuMath.h"
using namespace DirectX;
using namespace FukuMath;
using namespace Projection;
Camera::Camera()
{
	this->eye = Vector3(0.0f, 0.0f, -100.0f);
	this->target = Vector3(0.0f, 0.0f, 0.0f);
	this->up = Vector3(0.0f, 1.0f, 0.0f);
	this->right = Vector3(1.0f, 0.0f, 0.0f);
	this->shift = Vector3(0.0f, 0.0f, 0.0f);
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	matBillBoard = XMMatrixIdentity();
	matBillBoardY = XMMatrixIdentity();


}

void Camera::Init(const DirectX::XMFLOAT3 &eye, const DirectX::XMFLOAT3 &target, const DirectX::XMFLOAT3 &up, bool isPerspective, const Projection::ProjectionData &projectionData)
{
	this->eye = eye;
	this->target = target;
	this->up = up;
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	bool isDefault = (projectionData.height <= 0.0f || projectionData.width <= 0.0f);
	if (!isDefault)
	{
		this->projectionData = projectionData;
	}
	else
	{
		MyDirectX *myD = MyDirectX::Instance();

		this->projectionData.height = static_cast<float>(myD->winHeight);
		this->projectionData.width = static_cast<float>(myD->winWidth);
	}

	//������s��̐���

	if (isPerspective)
	{
		matProjection = DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(this->projectionData.angle),				//�㉺��p60�x
			(float)this->projectionData.width / (float)this->projectionData.height,	//�A�X�y�N�g��(��ʉ���/��ʏc��)
			this->projectionData.screenNear, this->projectionData.screenFar							//�O�[�A���[
		);
	}
	else
	{
		matProjection = DirectX::XMMatrixTranslation(0.0f, 0.0f, -this->projectionData.screenNear);
		matProjection *= DirectX::XMMatrixScaling((float)(1.0f / this->projectionData.width), (float)(1.0f / this->projectionData.height), (1.0f / this->projectionData.screenNear - this->projectionData.screenFar));
	}
}

void Camera::Update()
{
	//matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	//matView *= XMMatrixTranslation(position.x, position.y, position.z);
	MakeMatCamera();
	SetShift(XMFLOAT3(0, 0, 0));
}

void Camera::SetShift(XMFLOAT3 shift)
{
	this->shift = shift;
}

XMFLOAT3 Camera::GetScreenPos(const DirectX::XMFLOAT3 &pos)
{
	XMVECTOR v = {};
	XMFLOAT3 p = pos;
	XMStoreFloat3(&p, v);
	v = XMVector3Transform(v, matView);
	v = XMVector3Transform(v, matProjection);

	return XMFLOAT3(v.m128_f32[0], v.m128_f32[1], v.m128_f32[2]);
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
	XMFLOAT3 tmpEye = eye + shift;
	XMVECTOR eyePosition = XMLoadFloat3(&tmpEye);
	//�����_���W

	XMFLOAT3 tmpTarget = target + shift;
	XMVECTOR targetPosition = XMLoadFloat3(&tmpTarget);
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
	right = cameraAxisX;
	//�J������Y��
	XMVECTOR cameraAxisY;
	//Y����Z����X���̊O�ςŋ��܂�
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	//�x�N�g���̐��K��
	cameraAxisY = XMVector3Normalize(cameraAxisY);

	up = Vector3(cameraAxisY.m128_f32[0], cameraAxisY.m128_f32[1], cameraAxisY.m128_f32[2]);
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

DirectX::XMMATRIX Camera::GetMatViewProj()
{
	return (matView * matProjection);
}

DirectX::XMFLOAT3 Camera::GetAngle()
{
	return Vector3(target- eye).normalize();
}

DirectX::XMFLOAT3 Camera::GetRight()
{
	return right;
}

void Camera::CameraRot(const DirectX::XMVECTOR &rotQ)
{
	//target����camera�̎��_���W�܂ł̃x�N�g�����v�Z
	XMVECTOR camVec = XMLoadFloat3(&(eye - target));
	//��]������
	camVec = XMVector3Rotate(camVec, rotQ);

	Vector3 camPos;
	XMStoreFloat3(&camPos, camVec);
	//target�̍��W�{���������x�N�g�������킹�ĉ�]����
	camPos += target;
	eye = camPos;
}

void Camera::TargetRot(const DirectX::XMVECTOR &rotQ)
{
	//target����camera�̎��_���W�܂ł̃x�N�g�����v�Z
	XMVECTOR camVec = XMLoadFloat3(&(target - eye));
	//��]������
	camVec = XMVector3Rotate(camVec, rotQ);

	Vector3 camPos;
	XMStoreFloat3(&camPos, camVec);
	//target�̍��W�{���������x�N�g�������킹�ĉ�]����
	camPos += eye;
	target = camPos;
}

void Camera::UpVecReset()
{
	XMStoreFloat3(&up, YVec);
}

void Camera::MoveCamera(const DirectX::XMFLOAT3 &vec)
{
	eye += vec;
	target += vec;
}
