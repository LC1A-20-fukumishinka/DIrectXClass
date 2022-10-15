#include "GravityPlayer.h"
#include "ModelPhongPipeline.h"
#include "GameInput.h"
#include "FukuMath.h"
#include "../EaseClass.h"
#include "Planet.h"
#include "gameConstData.h"
#include "../ShadowPipeline.h"
#include "../Collision/Collision.h"
#include "DirectInput.h"
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
	drawObject.SetPosition(Vector3(YVec));

	shadowObject.Init();
	shadowObject.SetModel(model);
	shadowObject.SetPosition(Vector3(YVec));
	SetBasePlanet(planet);

	localHeight = 0;
	status = PlayerStatus::STAND;

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
	else if (GameInput::Instance()->LockOnInput())
	{//���b�N�I���p�̈ړ�
		LockOnUpdate();
	}
	else
	{//�ʏ���
		NormalUpdate();
	}

	shadowObject.SetPosition(drawObject.GetPosition());
	shadowObject.SetRotation(drawObject.GetRotQuaternion());
}

void GravityPlayer::Finalize()
{
}

void GravityPlayer::Draw()
{
	drawObject.Update();
	drawObject.modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
}

void GravityPlayer::ShadowDraw()
{
	shadowObject.Update();
	shadowObject.modelDraw(ShadowPipeline::Instance()->GetPipeLine());
}

void GravityPlayer::Move(bool isSetAngle)
{
	//switch (status)
	//{
	//case STAND:
	//	FloorMove(isSetAngle);
	//	break;
	//case JUMP:
	//	JumpMove(isSetAngle);
	//	break;
	//default:
	//	break;
	//}

	FloorMove(isSetAngle);

}

void GravityPlayer::FloorMove(bool isSetAngle)
{
	XMFLOAT2 stick = GameInput::Instance()->LStick();

#pragma region jump
	if (status == PlayerStatus::STAND && GameInput::Instance()->ATrigger())
	{
		jumpSpeed = jumpPower;
		status = PlayerStatus::JUMP;
	}

	if (status == PlayerStatus::JUMP)
	{
		localHeight += jumpSpeed;
		jumpSpeed += gravity;
		if (localHeight <= 0)
		{
			localHeight = 0;
			status = PlayerStatus::STAND;
		}
	}
#pragma endregion

	Vector3 stickInputVector3(stick.x, 0, stick.y);
	//�X�e�B�b�N�̓��͂��x�N�^�[�ɓ����
	Vector3 move = stickInputVector3;

	//�J�����̍s�񂩂���͂���ʂ̌����ɍ��킹�ċ���
	XMVECTOR moveV = XMLoadFloat3(&move);
	//XMVECTOR camRot = XMQuaternionRotationMatrix(cam->GetMatBillboard());

	XMVECTOR moveUp = drawObject.GetUpVec();
	XMVECTOR moveFront;
	GetMatRot(XMLoadFloat3(&cam->GetRight()), moveUp, moveFront);
	XMMATRIX moveMat = GetMatRot( moveUp , moveFront);
	XMVECTOR camRot = XMQuaternionRotationMatrix(moveMat);
	moveV = XMVector3Rotate(moveV, camRot);
	XMStoreFloat3(&move, moveV);
	move.normalize();

	move *= maxMoveSpeed;
	Vector3 nowPos(drawObject.GetPosition());

	Vector3 dist = (nowPos + move) - basePlanet.lock()->GetPos();

	float length = basePlanet.lock()->GetScale() + localHeight;
	//��{�̘f�����狗���𑪂���
	dist = dist.normalize() * length;
	drawObject.SetPosition(basePlanet.lock()->GetPos() + dist);
	Vector3 up = {0.0f, 1.0f, 0.0f};

	//���ʏ�ɂ���������
	up = drawObject.GetPosition() - basePlanet.lock()->GetPos();

	//�@������
	XMVECTOR upV = XMLoadFloat3(&up.normalize());


	XMVECTOR rightV = {};
	//�E�擾

	if (!isSetAngle)
	{
		rightV = XMVector3Cross(upV, drawObject.GetFrontVec());
	}
	else
	{
		rightV = XMVector3Cross(upV, moveV);
	}

	XMVECTOR frontV = {};

	frontV = XMVector3Cross(rightV, upV);

	//�ʏ�ړ����̌����̕ω�
	if (move.length() > 0.0f)
	{
		drawObject.SetRotationVector(frontV, upV);
	}
}

void GravityPlayer::JumpMove(bool isSetAngle)
{
	//�ŏI�I�ɒn��̏����ƕ������邩���荇��k�Y�L�q���Ƃ���
	//if (status == PlayerStatus::JUMP)
	//{
	//	localHeight += jumpSpeed;
	//	jumpSpeed += gravity;
	//	if (localHeight <= 0)
	//	{
	//		localHeight = 0;
	//		status = PlayerStatus::STAND;
	//	}
	//}
}

void GravityPlayer::PlayerRotation()
{
	//�v���C���[�̃��[�J����X���ɑ΂��ĉ�]
	drawObject.AddRotation(XMQuaternionRotationAxis(drawObject.GetRightVec(), GameInput::Instance()->RStick().y * RotRate));
	//ToDo///////////////
	//���E��Y���łȂ������̗����Ă��鐂�������ɍŏI�I�ɕύX��������
	////////////////////
	drawObject.AddRotation(XMQuaternionRotationAxis(drawObject.GetUpVec(), GameInput::Instance()->RStick().x * RotRate));

}

void GravityPlayer::NormalUpdate()
{
	//�t�B�[���h�����̂ɂȂ����炻��ɍ��킹�ĕω�

	if (GameInput::Instance()->LockOnRelease())
	{
		PostureReset();
	}
	Move(true);
}

void GravityPlayer::PostureReset()
{
	Vector3 BasePlanetToPlayer = drawObject.GetPosition() - basePlanet.lock()->GetPos();
	XMVECTOR BasePlanetToPlayerAngleV = XMLoadFloat3(&BasePlanetToPlayer.normalize());
	XMVECTOR frontVec = XMVector3Cross(drawObject.GetRightVec(), BasePlanetToPlayerAngleV);

	drawObject.SetRotationVector(frontVec, BasePlanetToPlayerAngleV);
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
	shadowObject.SetModel(model);
}

void GravityPlayer::SetCamera(Camera *cam)
{
	this->cam = cam;
	drawObject.SetCamera(cam);
}

void GravityPlayer::SetShadowCamera(Camera *cam)
{
	shadowObject.SetCamera(cam);
}

void GravityPlayer::SetLight(LightGroup *lights)
{
	drawObject.SetLightGroup(lights);
	shadowObject.SetLightGroup(lights);
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

const XMFLOAT3 GravityPlayer::GetUpVec()
{
	XMFLOAT3 playerUp;
	XMStoreFloat3(&playerUp, drawObject.GetUpVec());
	return playerUp;
}

const float GravityPlayer::GetBasePlanetScale()
{
	if (basePlanet.expired())
	{
		return 0.0f;
	}
	return basePlanet.lock()->GetScale();
}

const std::weak_ptr<Planet> &GravityPlayer::GetBasePlanet()
{
	return basePlanet;
}

const bool GravityPlayer::GetIsJump()
{
	bool isJump = status == PlayerStatus::JUMP;
	return isJump;
}

void GravityPlayer::SetGrabPlanet(std::shared_ptr<Planet> planet)
{
	if (grabPlanet.expired())
	{
		this->grabPlanet = planet;
		Vector3 planetPlayerDistance = grabPlanet.lock()->GetPos() - drawObject.GetPosition();
		baseLength = planetPlayerDistance.length();
	}

	if (!grabPlanet.expired())
	{
		grabPlanet.lock()->GrabOn();
		XMFLOAT3 dist = grabPlanet.lock()->GetPos() - drawObject.GetPosition();
		XMVECTOR vec = XMLoadFloat3(&dist);
		//�f���Ɍ�������
		drawObject.SetRotationVector(vec, drawObject.GetUpVec());
		grabPlanet.lock()->SetGrabRotateAngle(drawObject.GetUpVec(), drawObject.GetRightVec());
	}
}

void GravityPlayer::SetBasePlanet(std::shared_ptr<Planet> planet)
{
	//���̘f����
	if (!this->basePlanet.expired())
	{
		//�����̒���
		float basePlanetToPlayerLength = (drawObject.GetPosition() - planet->GetPos()).length();

		//�f���̔��a�������č�������
		basePlanetToPlayerLength -= planet->GetScale();

		localHeight = basePlanetToPlayerLength;

		//�W�����v�̑��x�C��
		jumpSpeed = 0;
		this->basePlanet.lock()->ReleaseBase();
	}

	this->basePlanet = planet;
	basePlanet.lock()->OnPlayer();
	PostureReset();

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
	//�Ƃ肠���������Ȃ��悤�ɂ��܂��o�O�邩���
	//Move(false);

	//���ƃv���C���[�̓�_�Ԃ̋������v�Z
	Vector3 planetPlayerDistance = grabPlanet.lock()->GetPos() - drawObject.GetPosition();



	const float moveLengthSpeed = 0.1f;
	if (GameInput::Instance()->B())
	{
		baseLength -= moveLengthSpeed;
	}

	if (GameInput::Instance()->X())
	{
		baseLength += moveLengthSpeed;
	}
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
	grabPlanet.lock()->SetGrabRotateAngle(drawObject.GetUpVec(), drawObject.GetRightVec());

}

void GravityPlayer::BlockCollision(const std::vector<Triangle> &boxPlanes)
{
	//�v���C���[�̗�����ray
	XMVECTOR downVec = -drawObject.GetUpVec();
	Ray playerDownVec;
	playerDownVec.dir = downVec;
	playerDownVec.start = XMLoadFloat3(&(drawObject.GetPosition() + (drawObject.GetUpVec() * 0.5f)));

	for (auto &e : boxPlanes)
	{
		float distance = 1000;
		DirectX::XMVECTOR onPlayerPos;
		bool isOnPlayer = false;
		//��锻��
		//�@���������v���C���[�̏d�͕����Ɠ��ς��Ƃ��ĂP�ɋ߂����ǂ����𔻒�
		if (XMVector3Dot(downVec, -e.normal).m128_f32[0] >= 0.7f)
		{
		//��Ƒ���̐ڐG����
			isOnPlayer = Collision::CheckRay2Triangle(playerDownVec, e, &distance, &onPlayerPos);
		}

		if (isOnPlayer)
		{
			int hoge = 1;
		}
		if (isOnPlayer && distance <= 1.1f)
		{
			XMVECTOR identity = {2.0f,2.0f ,2.0f ,0.0f };
			onPlayerPos += identity * drawObject.GetUpVec();
			drawObject.SetPosition(Vector3(onPlayerPos));
			jumpSpeed = 0.0f;
		}
		////�����Ԃ�����
		//Collision::CheckSphere2Triangle();
	}
}
