#include "GravityPlayer.h"
#include "ModelPhongPipeline.h"
#include "GameInput.h"
#include "FukuMath.h"
#include "../EaseClass.h"
#include "../Planet.h"
using namespace GameInput;
using namespace DirectX;
using namespace FukuMath;
static float testRot = 0;
static Easing RotX;
static Easing RotY;
bool isX = false;
bool isY = false;
void GravityPlayer::Init(Model *model)
{
	drawObject.Init();
	drawObject.SetModel(model);
}

void GravityPlayer::Update()
{
	XMFLOAT2 stick = LStick();
	Vector3 move = { stick.x,0, stick.y };
	Vector3 nowPos(drawObject.GetPosition());
	drawObject.SetPosition(nowPos + move);

	GrabUpdate();
}

void GravityPlayer::Finalize()
{
}

void GravityPlayer::Draw()
{
	drawObject.Update();
	drawObject.modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
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
	drawObject.SetCamera(cam);
}

void GravityPlayer::SetLight(LightGroup *lights)
{
	drawObject.SetLightGroup(lights);
}

const DirectX::XMFLOAT3 GravityPlayer::GetPos()
{
	return drawObject.GetPosition();
}

void GravityPlayer::SetGrabPlanet(std::shared_ptr<Planet> planet)
{
	this->grabPlanet = planet;
	grabPlanet.lock()->GrabInput();
	XMFLOAT3 dist = grabPlanet.lock()->GetPos() - drawObject.GetPosition();
	XMVECTOR vec = XMLoadFloat3(&dist);
	//�f���Ɍ�������
	drawObject.SetRotationVector(vec);
	grabPlanet.lock()->SetGrabRotateAngle(drawObject.GetUpVec());
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
	bool isNotGrab = grabPlanet.expired();
	if (isNotGrab)return;

	//���ƃv���C���[�̓�_�Ԃ̋������v�Z
	Vector3 planetPlayerDistance = grabPlanet.lock()->GetPos() - drawObject.GetPosition();

	//��_�Ԃ̒������v�Z
	float length = planetPlayerDistance.length();

	//�v���C���[�̃��[�J����X���ɑ΂��ĉ�]
	drawObject.AddRotation(XMQuaternionRotationAxis(drawObject.GetRightVec(), RStick().y * degree));
	//ToDo///////////////
	//���E��Y���łȂ������̗����Ă��鐂�������ɍŏI�I�ɕύX��������
	////////////////////
	//���E��Y���ɑ΂��ĉ�]
	drawObject.AddRotation(XMFLOAT3{ 0.0f, RStick().x * degree, 0.0});

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

}
