#include "Player.h"
#include "ModelPhongPipeline.h"
#include "GameInput.h"
#include "FukuMath.h"
#include "../EaseClass.h"
#include "Planet.h"
#include "PlanetManager.h"
#include "../ShadowPipeline.h"
#include "../Collision/Collision.h"
#include "DirectInput.h"
#include "../Shake.h"
using namespace DirectX;
using namespace FukuMath;
using namespace GameDatas;
static float testRot = 0;
static Easing RotX;
static Easing RotY;
bool isX = false;
bool isY = false;
void Player::Init(std::shared_ptr<Planet> planet)
{

	pos_;
	drawObject.Init();
	drawObject.SetModel(playerModel_.get());
	pos_ = Vector3(YVec);
	drawObject.SetPosition(pos_);

	shadowObject.Init();
	shadowObject.SetModel(playerModel_.get());
	shadowObject.SetPosition(Vector3(YVec));
	SetBasePlanet(planet);

	localHeight = 0;
	status_ = PlayerStatus::STAND;
	oldStatus_ = status_;
	if (!basePlanet.expired())
	{
		Vector3 startPos;
		startPos = basePlanet.lock()->GetPos();
		startPos += YVec * basePlanet.lock()->GetScale();
		pos_ = startPos;
		drawObject.SetPosition(pos_);
	}

	gravity_.isOneWayGravity = false;

	//���ׂĂ̂��イ��傭���������ŏ�����						   ������
	oneWayGravityAngle_ = gravity_.angle = worldGravity_ = -YVec;
}

void Player::Update()
{
	//�f����͂�ł��Ȃ����ǂ���
	bool isNotGrab = grabPlanet.expired();

	oldStatus_ = status_;

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


	if (status_ == PlayerStatus::STAND)
	{
		PlanetManager::Instance()->playerStand(basePlanet);
	}
	drawObject.SetPosition(pos_);

	shadowObject.SetPosition(drawObject.GetPosition());
	shadowObject.SetRotation(drawObject.GetRotQuaternion());
}

void Player::Finalize()
{
}

void Player::Draw()
{
	drawObject.Update();
	drawObject.modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
}

void Player::ShadowDraw()
{
	shadowObject.Update();
	shadowObject.modelDraw(ShadowPipeline::Instance()->GetPipeLine());
}

void Player::Move(bool isSetAngle)
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

	//�d�͑��쎞
	if (GameInput::Instance()->GrabTrigger())
	{
		//���イ��傭���������鎞
		if (gravity_.isOneWayGravity)
		{//���x���[���ɂ���
			moveVec_ *= 0.3f;
		}
		gravity_.isOneWayGravity = !gravity_.isOneWayGravity;
		if (GameInput::Instance()->LockOnInput())
		{
			oneWayGravityAngle_ = cam->GetAngle();
		}
		else
		{
			oneWayGravityAngle_ = cam->GetAngle();
		}
	}
	FloorMove(isSetAngle);

}

void Player::PosUpdate(const Vector3 &move)
{
	Vector3 shakeVec;

#pragma region jump
	//�W�����v���̏���
	if (status_ == PlayerStatus::JUMP)
	{
		//���ƃv���C���[�̋���
		Vector3 dist = pos_ - basePlanet.lock()->GetPos();


		//�����ɗ������鐬��
		moveVec_ += (gravity_.angle * baseGravityPower);

		//�o����ō����x
		const float maxSpeed = 3.0f;
		//�J�������V�F�C�N���n�߂鑬�x
		const float shakeSpeed = 2.0f;

		//���݂̑��x
		float nowSpeed = moveVec_.length();
		if (nowSpeed >= shakeSpeed)
		{
			float shakePower = (0.1f * (nowSpeed - shakeSpeed));
			shakeUpdate(shakePower);
		}


		if (nowSpeed >= maxSpeed)
		{
			moveVec_ = (moveVec_.normalize() * maxSpeed);
		}

		moveVec_ += (warkVec_ * 0.02f);

		if (dist.length() <= basePlanet.lock()->GetScale())
		{
			if (nowSpeed >= shakeSpeed)
			{
				shakeTimer_ = 0;
				//�d�͕ύX������
				isGravityChanged_ = true;
			}
			status_ = PlayerStatus::STAND;
		}
	}

	if (shakeTimer_ <= shakeTimerMax_)
	{
		float shakePower = 3.0f * (1.0f - (static_cast<float> (shakeTimer_) / shakeTimerMax_));
		shakeUpdate(shakePower);
		shakeTimer_++;
	}

	//�W�����v�����u��
	if (status_ == PlayerStatus::STAND && GameInput::Instance()->ATrigger())
	{
		//���ʂɑ΂��Ă̖@������
		Vector3 JumpAngle;

		//�����Ă���ʂ̐��������ɒ���
		Vector3 dist = pos_ - basePlanet.lock()->GetPos();
		JumpAngle = (dist.normalize());
		//���������ɉ���
		moveVec_ = (JumpAngle * jumpPower);

		moveVec_ += warkVec_;
		//�W�����v��ԂɈڍs
		status_ = PlayerStatus::JUMP;
	}


#pragma endregion

	//�f���Ɉ����񂹂����Ԃ�
	bool isPlanetAttracts = !(status_ == PlayerStatus::JUMP && gravity_.isOneWayGravity);

	//���݈ʒu
	Vector3 nowPos(pos_);

	//�㗤���̘f���Ƃ̋���
	Vector3 dist = (nowPos + (move * maxMoveSpeed)) - basePlanet.lock()->GetPos();

	//�ړ���̍��W���v�Z�i���݈ʒu�ɒ���ړ��Ɛ����ړ��̑��x�����Z�j
	nowPos += (moveVec_);


	//�n��ɗ����Ă���Ƃ����ʂɈʒu��␳����
	if (status_ == PlayerStatus::STAND)
	{
		float length = basePlanet.lock()->GetScale();

		//��{�̘f�����狗���𑪂��ċ��ʂɉ����Ĉړ�
		dist = dist.normalize() * length;

		//���W����
		nowPos = (basePlanet.lock()->GetPos() + dist);
	}

	pos_ = nowPos;
}

void Player::PostureUpdate(const Vector3 &move)
{


	//���ʏ�ɂ���������
	Vector3 up;

	if (status_ == PlayerStatus::STAND)
	{
		up = Vector3(pos_ - basePlanet.lock()->GetPos()).normalize();
	}
	else
	{
		up = -gravity_.angle;
	}
	//�x�N�^�[�^�ɂ���
	XMVECTOR upV = XMLoadFloat3(&up.normalize());

	//�E
	XMVECTOR rightV = {};

	rightV = XMVector3Cross(upV, drawObject.GetFrontVec());

	//����
	XMVECTOR frontV = {};
	//�ړ����Ă��Ȃ���Ԃł��p����������邽�߂�if���˂�

	if (move.length() > 0.0f)
	{
		rightV = XMVector3Cross(upV, XMLoadFloat3(&move));

		frontV = XMVector3Cross(rightV, upV);
	}
	else
	{
		rightV = XMVector3Cross(upV, drawObject.GetFrontVec());

		frontV = XMVector3Cross(drawObject.GetRightVec(), upV);
	}

	//�ʏ�ړ����̌����̕ω�
	drawObject.SetRotationVector(frontV, upV);

}

void Player::FloorMove(bool isSetAngle)
{
	XMFLOAT2 stick = GameInput::Instance()->LStick();

	Vector3 stickInputVector3(stick.x, 0, stick.y);
	//�X�e�B�b�N�̓��͂��x�N�^�[�ɓ����
	Vector3 move = stickInputVector3;

	//�J�����̍s�񂩂���͂���ʂ̌����ɍ��킹�ċ���

	XMVECTOR moveV = XMLoadFloat3(&move);

	//���͂̎p�������
	XMVECTOR moveUp = drawObject.GetUpVec();
	XMVECTOR moveFront = XMLoadFloat3(&Vector3(cam->GetRight()).cross(moveUp));
	XMMATRIX moveMat = GetMatRot(moveUp, moveFront);

	//�p���ɍ��킹�ăx�N�g����z�u
	XMVECTOR moveRot = XMQuaternionRotationMatrix(moveMat);
	moveV = XMVector3Rotate(moveV, moveRot);
	XMStoreFloat3(&move, moveV);

	//�ꉞ���K��
	move.normalize();

	//���s�ړ�
	warkVec_ = (move * maxMoveSpeed);



	if (gravity_.isOneWayGravity)
	{
		gravity_.angle = oneWayGravityAngle_;
	}
	else if (worldGravity_.length() > 0.0f)
	{

		gravity_.angle = worldGravity_;
	}
	else
	{
		Vector3 dist = pos_ - basePlanet.lock()->GetPos();
		gravity_.angle = -(dist.normalize());
	}
	PosUpdate(move);

	if (isSetAngle)
	{
		PostureUpdate(move);
	}
}

void Player::JumpMove(bool isSetAngle)
{

}

void Player::PlayerRotation()
{
	//�v���C���[�̃��[�J����X���ɑ΂��ĉ�]
	drawObject.AddRotation(XMQuaternionRotationAxis(drawObject.GetRightVec(), GameInput::Instance()->RStick().y * RotRate));

	//���E��Y���łȂ������̗����Ă��鐂�������ɍŏI�I�ɕύX��������
	drawObject.AddRotation(XMQuaternionRotationAxis(drawObject.GetUpVec(), GameInput::Instance()->RStick().x * RotRate));

}

void Player::NormalUpdate()
{
	//�t�B�[���h�����̂ɂȂ����炻��ɍ��킹�ĕω�

	if (GameInput::Instance()->LockOnRelease())
	{
		PostureReset();
	}
	Move(true);
}

void Player::PostureReset()
{
	if (status_ == PlayerStatus::JUMP) return;
	Vector3 BasePlanetToPlayer = pos_ - basePlanet.lock()->GetPos();
	XMVECTOR BasePlanetToPlayerAngleV = XMLoadFloat3(&BasePlanetToPlayer.normalize());
	XMVECTOR frontVec = XMVector3Cross(drawObject.GetRightVec(), BasePlanetToPlayerAngleV);

	drawObject.SetRotationVector(frontVec, BasePlanetToPlayerAngleV);
}

void Player::AddGravity(Vector3 gravity)
{
	worldGravity_ = gravity;
}

void Player::shakeUpdate(float shakePower)
{
	cam->SetShift(Shake::GetShake(shakePower));
}

void Player::LoadModel()
{
	playerModel_ = std::make_unique<Model>();
	playerModel_->CreateModel("chr_sword", true);

	ArrowModel_ = std::make_unique<Model>();
	ArrowModel_->CreateModel("chr_sword", true);
}

bool Player::LandingCamReset()
{
	bool isReset = (status_ == PlayerStatus::STAND && oldStatus_ == PlayerStatus::JUMP && isGravityChanged_);

	if (isGravityChanged_)
	{
		isGravityChanged_ = false;
	}
	return isReset;
}

void Player::LockOnUpdate()
{
	Move(false);
	PlayerRotation();
}

void Player::SetPos(const DirectX::XMFLOAT3 &pos)
{
	pos_ = pos;
	drawObject.SetPosition(pos_);
}

void Player::SetRotation(const DirectX::XMFLOAT3 &rot)
{
	drawObject.SetRotation(rot);
}

void Player::SetModel(Model *model)
{
	drawObject.SetModel(model);
	shadowObject.SetModel(model);
}

void Player::SetCamera(Camera *cam)
{
	this->cam = cam;
	drawObject.SetCamera(cam);
}

void Player::SetShadowCamera(Camera *cam)
{
	shadowObject.SetCamera(cam);
}

void Player::SetLight(LightGroup *lights)
{
	drawObject.SetLightGroup(lights);
	shadowObject.SetLightGroup(lights);
}

const DirectX::XMFLOAT3 &Player::GetPos()
{
	return pos_;
}

const XMFLOAT3 Player::GetAngle()
{
	XMFLOAT3 angle;
	XMStoreFloat3(&angle, drawObject.GetFrontVec());
	return angle;
}

const XMFLOAT3 Player::GetUpVec()
{
	XMFLOAT3 playerUp;
	XMStoreFloat3(&playerUp, drawObject.GetUpVec());
	return playerUp;
}

float Player::GetBasePlanetScale()
{
	if (basePlanet.expired())
	{
		return 0.0f;
	}
	return basePlanet.lock()->GetScale();
}

const std::weak_ptr<Planet> &Player::GetBasePlanet()
{
	return basePlanet;
}

GameDatas::GravityData Player::GetGravityData()
{
	return gravity_;
}

bool Player::GetIsJump()
{
	bool isJump = status_ == PlayerStatus::JUMP;
	return isJump;
}

bool Player::GetIsOneWayGravity()
{
	return gravity_.isOneWayGravity;
}

void Player::SetGrabPlanet(std::shared_ptr<Planet> planet)
{
	if (grabPlanet.expired())
	{
		this->grabPlanet = planet;
		Vector3 planetPlayerDistance = grabPlanet.lock()->GetPos() - pos_;
		baseLength = planetPlayerDistance.length();
	}

	if (!grabPlanet.expired())
	{
		grabPlanet.lock()->GrabOn();
		XMFLOAT3 dist = grabPlanet.lock()->GetPos() - pos_;
		XMVECTOR vec = XMLoadFloat3(&dist);
		//�f���Ɍ�������
		drawObject.SetRotationVector(vec, drawObject.GetUpVec());
		grabPlanet.lock()->SetGrabRotateAngle(drawObject.GetUpVec(), drawObject.GetRightVec());
	}
}

void Player::SetBasePlanet(std::shared_ptr<Planet> planet)
{
	//���̘f����
	if (!this->basePlanet.expired())
	{
		//�����̒���
		float basePlanetToPlayerLength = (pos_ - planet->GetPos()).length();

		//�f���̔��a�������č�������
		basePlanetToPlayerLength -= planet->GetScale();

		localHeight = basePlanetToPlayerLength;

		//�W�����v�̑��x�C��
		this->basePlanet.lock()->ReleaseBase();
	}

	this->basePlanet = planet;
	basePlanet.lock()->OnPlayer();
	PostureReset();

}

void Player::ReleasePlanet()
{
	bool isNotGrab = grabPlanet.expired();
	if (isNotGrab)return;
	grabPlanet.lock()->ReleaseGrab();
	grabPlanet.reset();
}

void Player::GrabUpdate()
{
	//�Ƃ肠���������Ȃ��悤�ɂ��܂��o�O�邩���
	//Move(false);

	//���ƃv���C���[�̓�_�Ԃ̋������v�Z
	Vector3 planetPlayerDistance = grabPlanet.lock()->GetPos() - pos_;



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
	XMVECTOR playerPos = XMLoadFloat3(&pos_);
	vec += playerPos;

	//�f���̈ʒu����
	XMFLOAT3 planetPos = {};
	XMStoreFloat3(&planetPos, vec);
	grabPlanet.lock()->SetPos(planetPos);
	grabPlanet.lock()->SetGrabRotateAngle(drawObject.GetUpVec(), drawObject.GetRightVec());

}

void Player::BlockCollision(const std::vector<Triangle> &boxPlanes)
{
	//�v���C���[�̗�����ray
	XMVECTOR downVec = -drawObject.GetUpVec();
	Ray playerDownVec;
	playerDownVec.dir = downVec;
	playerDownVec.start = XMLoadFloat3(&(pos_ + (drawObject.GetUpVec() * 0.5f)));

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
			XMVECTOR identity = { 2.0f,2.0f ,2.0f ,0.0f };
			onPlayerPos += identity * drawObject.GetUpVec();
			pos_ = Vector3(onPlayerPos);
		}
		////�����Ԃ�����
		//Collision::CheckSphere2Triangle();
	}
}
