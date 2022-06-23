#include "GravityPlayer.h"
#include "ModelPhongPipeline.h"
#include "GameInput.h"
#include "FukuMath.h"
#include "../EaseClass.h"
#include "Planet.h"
#include "gameConstData.h"
using namespace GameInput;
using namespace DirectX;
using namespace FukuMath;
using namespace GameDatas;
static float testRot = 0;
static Easing RotX;
static Easing RotY;
bool isX = false;
bool isY = false;
void GravityPlayer::Init(Model *model, std::shared_ptr<Planet> planet)
{
	drawObject.Init();
	drawObject.SetModel(model);
	SetBasePlanet(planet);

	if (!basePlanet.expired())
	{
		Vector3 startPos;
		startPos = basePlanet.lock()->GetPos();
		startPos += YVec * basePlanet.lock()->GetScale();
		drawObject.SetPosition(startPos);
	}
}

void GravityPlayer::Update()
{
	//�f����͂�ł��Ȃ����ǂ���
	bool isNotGrab = grabPlanet.expired();

	if (!isNotGrab)
	{//�͂�ł���
		GrabUpdate();
	}
	else if (LockOnInput())
	{//���b�N�I���p�̈ړ�
		LockOnUpdate();
	}
	else
	{//�ʏ���
		NormalUpdate();
	}
}

void GravityPlayer::Finalize()
{
}

void GravityPlayer::Draw()
{
	drawObject.Update();
	drawObject.modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
}

void GravityPlayer::Move(bool isSetAngle)
{
	XMFLOAT2 stick = LStick();
	//�X�e�B�b�N�̓��͂��x�N�^�[�ɓ����
	Vector3 move = { 0,0, stick.y };

	//�J�����̍s�񂩂���͂���ʂ̌����ɍ��킹�ċ���
	XMVECTOR moveV = XMLoadFloat3(&move);
	XMVECTOR camRot = XMQuaternionRotationMatrix(cam->GetMatBillboardY());
	moveV = XMVector3Rotate(moveV, camRot);
	XMStoreFloat3(&move, moveV);

	Vector3 up = {};

	up = drawObject.GetPosition() - basePlanet.lock()->GetPos();

	//�@������
	XMVECTOR upV = XMLoadFloat3(&up.normalaize());


	XMVECTOR rightV = {};
	//�E�擾
	rightV = XMVector3Cross(upV, moveV);

	XMVECTOR frontV = {};

	frontV = XMVector3Cross(rightV, upV);
	//Y���ړ����C��
	//move.y = 0;
	Vector3 nowPos(drawObject.GetPosition());

	//�����̕ω�
	if (isSetAngle && move.length() > 0.0f)
	{
		XMVECTOR inputVec = XMLoadFloat3(&move);
		drawObject.SetRotationVector(inputVec);
	}
}

void GravityPlayer::PlayerRotation()
{
	//�v���C���[�̃��[�J����X���ɑ΂��ĉ�]
	drawObject.AddRotation(XMQuaternionRotationAxis(drawObject.GetRightVec(), RStick().y * RotRate));
	//ToDo///////////////
	//���E��Y���łȂ������̗����Ă��鐂�������ɍŏI�I�ɕύX��������
	////////////////////

	//���E��Y���ɑ΂��ĉ�]
	drawObject.AddRotation(XMFLOAT3{ 0.0f, RStick().x * RotRate, 0.0 });
}

void GravityPlayer::NormalUpdate()
{
	//�t�B�[���h�����̂ɂȂ����炻��ɍ��킹�ĕω�
	XMVECTOR angle = drawObject.GetFrontVec();
	angle.m128_f32[1] = 0.0f;
	drawObject.SetRotationVector(angle);
	Move(true);
}

void GravityPlayer::LockOnUpdate()
{
	Move(false);
	PlayerRotation();
}

void GravityPlayer::SetPos(const DirectX::XMFLOAT3 &pos)
{
	drawObject.SetPosition(pos);
}

void GravityPlayer::SetRotation(const DirectX::XMFLOAT3 &rot)
{
	drawObject.SetRotation(rot);
}

void GravityPlayer::SetModel(Model *model)
{
	drawObject.SetModel(model);
}

void GravityPlayer::SetCamera(Camera *cam)
{
	this->cam = cam;
	drawObject.SetCamera(cam);
}

void GravityPlayer::SetLight(LightGroup *lights)
{
	drawObject.SetLightGroup(lights);
}

const DirectX::XMFLOAT3 &GravityPlayer::GetPos()
{
	return drawObject.GetPosition();
}

const XMFLOAT3 GravityPlayer::GetAngle()
{
	XMFLOAT3 angle;
	XMStoreFloat3(&angle, drawObject.GetFrontVec());
	return angle;
}

void GravityPlayer::SetGrabPlanet(std::shared_ptr<Planet> planet)
{
	if (grabPlanet.expired())
	{
		this->grabPlanet = planet;
		Vector3 planetPlayerDistance = grabPlanet.lock()->GetPos() - drawObject.GetPosition();
		baseLength = planetPlayerDistance.length();
	}
	grabPlanet.lock()->GrabInput();
	XMFLOAT3 dist = grabPlanet.lock()->GetPos() - drawObject.GetPosition();
	XMVECTOR vec = XMLoadFloat3(&dist);
	//�f���Ɍ�������
	drawObject.SetRotationVector(vec);
	grabPlanet.lock()->SetGrabRotateAngle(YVec, drawObject.GetRightVec());
}

void GravityPlayer::SetBasePlanet(std::shared_ptr<Planet> planet)
{
	this->basePlanet = planet;
}

void GravityPlayer::ReleasePlanet()
{
	bool isNotGrab = grabPlanet.expired();
	if (isNotGrab)return;
	grabPlanet.lock()->ReleaseGrab();
	grabPlanet.reset();
}

void GravityPlayer::GrabUpdate()
{
	Move(false);

	//���ƃv���C���[�̓�_�Ԃ̋������v�Z
	Vector3 planetPlayerDistance = grabPlanet.lock()->GetPos() - drawObject.GetPosition();

	//��_�Ԃ̒������v�Z
	float length = baseLength;
	if (false)
	{
#pragma region Loap
		length = planetPlayerDistance.length();
		//��_�Ԃ̋������͂񂾏u�Ԃ��Z����
		if (length <= baseLength)
		{//�Z���ꍇ
			drawObject.SetRotationVector(XMLoadFloat3(&planetPlayerDistance));
		}
		else
		{
			length = baseLength;
		}
#pragma endregion
	}

	PlayerRotation();

	//�v���C���[�̐��ʕ�����
	XMVECTOR vec = drawObject.GetFrontVec();

	//�ړ��ʂ���Z����
	vec *= length;

	//������v���C���[�̈ʒu�ɉ��Z����
	XMVECTOR playerPos = XMLoadFloat3(&drawObject.GetPosition());
	vec += playerPos;

	//�f���̈ʒu����
	XMFLOAT3 planetPos = {};
	XMStoreFloat3(&planetPos, vec);
	grabPlanet.lock()->SetPos(planetPos);
	grabPlanet.lock()->SetGrabRotateAngle(YVec, drawObject.GetRightVec());

}
