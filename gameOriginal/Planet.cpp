#include "Planet.h"
#include "../Camera.h"
#include "../LightGroup.h"
#include "ModelPhongPipeline.h"
#include "../ShadowDrawPipeline.h"
#include "FukuMath.h"
#include "GameInput.h"
#include "gameConstData.h"
#include <algorithm>
using namespace std;
using namespace FukuMath;
using namespace DirectX;
using namespace GameDatas;
Camera *Planet::cam = nullptr;
Camera *Planet::shadowCam = nullptr;
int Planet::shadowTextureNum;
LightGroup *Planet::lights = nullptr;
Vector3 Planet::sPlayerPos = Vector3();
Planet::Planet()
{
	model = make_unique<Model>();

	model->CreateModel("sphere", true, true);
	//model->CreateModel("chr_sword");//回転確認用
	object = make_unique<PlanetObject>();
	object->Init();
	object->SetModel(model.get());
	object->SetCamera(Planet::cam);
	object->SetScale(XMFLOAT3(scale, scale, scale));
	object->SetShadowCamera(Planet::shadowCam);
	object->SetShadowTextureNum(shadowTextureNum);
	GrabRotateAxisY = YVec;
	GrabRotateAxisX = XVec;
}

Planet::~Planet()
{
}

void Planet::Init(const DirectX::XMFLOAT3 &pos, float size, const DirectX::XMFLOAT4 &color, int stageID, bool Spawn, const PlanetType &type)
{
	type_ = type;
	ID_ = stageID;
	startPos_ = pos;
	startScale_ = size;
	startColor_ = color;
	startIsSpawn_ = Spawn;
	SpawnAnimationEase_.Init(20);
	object->SetCamera(cam);
	object->SetLightGroup(lights);
	object->SetShadowCamera(shadowCam);
	object->SetShadowTextureNum(shadowTextureNum);
	Reset();
}

void Planet::Update()
{
	if (isSpawnAnimation_)
	{
		SpawnAnimationUpdate();
	}

	if (!isGrab_)
	{
		NormalUpdate();
	}
	else
	{
		GrabUpdate();
	}

	if (isColorChange_)
	{
		float t = 0.25f;
		t /= (startScale_ * 2);
		clearRate += t;
		colorRate = 0.0f;
		clearRate = std::clamp(clearRate, 0.0f, 1.0f);
		colorRate = std::clamp(colorRate, 0.0f, 1.0f);
	}
	else
	{
		if (isNext_)
		{
			colorRate += 0.03f;
			if (colorRate >= 1.0f)
			{
				colorRate = 0.0f;
				isNextLightup_ = !isNextLightup_;
			}
		}
		else
		{
			colorRate = 0.0f;
		}

		ColorOnAngle = Vector3(sPlayerPos - pos).normalize();
	}
}

void Planet::NormalUpdate()
{
	//Vector3 tmp = {};
	//tmp.y += degree;
	//object->AddRotation(tmp);
	object->SetPosition(pos);
	object->SetCamera(cam);
	object->SetLightGroup(lights);
	object->SetShadowCamera(shadowCam);
	object->SetShadowTextureNum(shadowTextureNum);
}

void Planet::GrabUpdate()
{
	XMVECTOR Rot = XMQuaternionRotationAxis(GrabRotateAxisY, GameInput::Instance()->RStick().x * RotRate);
	Rot = XMQuaternionMultiply(Rot, XMQuaternionRotationAxis(GrabRotateAxisX, GameInput::Instance()->RStick().y * RotRate));
	object->AddRotation(Rot);
	object->SetCamera(cam);
	object->SetLightGroup(lights);
	object->SetPosition(pos);
	object->SetShadowCamera(shadowCam);
	object->SetShadowTextureNum(shadowTextureNum);
}

void Planet::SpawnAnimationUpdate()
{
	//設定した通常のサイズまで拡大していく
		//イージングしていく
	float rate = SpawnAnimationEase_.Do(Easing::EaseMove::Out, Easing::Type::Back);

	//いーじんぐの値を使ってスケーリング
	float scaleRate = rate * startScale_;

	//用意したスケールをobjectに適用
	SetScale(scaleRate);


	//拡大の演出が終了したら存在フラグをオンにする
		//イージングの処理が全て終了したら
	if (SpawnAnimationEase_.IsEnd())
	{
		//アニメーションをするフラグをオフに
		isSpawnAnimation_ = false;
		//生存フラグをオンに
		isSpawn_ = true;
	}

}

void Planet::SpawnAnimationStart()
{
	//アニメーション開始フラグをオンにする
	isSpawnAnimation_ = true;
	//アニメーション開始時に設定する必要があるものはここに記述する
		//イージングの初期化処理(またリセット処理)
	SpawnAnimationEase_.Reset();
}

void Planet::Draw()
{

	object->Update();

	float easeRate = 0.0f;
	if (!isColorChange_)
	{
		if (isNextLightup_)
		{
			easeRate = Easing::easeOutCubic(colorRate);
		}
		else
		{
			easeRate = Easing::easeInCubic(1.0f - colorRate);
		}
		easeRate *= 0.3f;
	}
	object->SendPlanetAnimationData(ColorOnAngle, clearRate, easeRate);
	object->modelDraw(ShadowDrawPipeline::Instance()->GetPipeLine());
}

void Planet::Finalize()
{
}

void Planet::SetScale(float scale)
{
	this->scale = scale;
	object->SetScale(XMFLOAT3(scale, scale, scale));
}

Object3D *Planet::GetObject3d()
{
	return object.get();
}

float Planet::GetScale()
{
	return scale;
}

float Planet::GetStartScale()
{
	return startScale_;
}

bool Planet::GetIsSpawn()
{
	return isSpawn_;
}

bool Planet::GetIsSpawnAnimationEnd()
{
	return SpawnAnimationEase_.IsEnd();
}

void Planet::ColorChange()
{
	isColorChange_ = true;
}

void Planet::ColorOff()
{
	isColorChange_ = false;
	clearRate = 0.0f;
}

bool Planet::GetIsColorChange()
{
	return isColorChange_;
}

void Planet::SetLight(LightGroup *lights)
{
	Planet::lights = lights;
}

void Planet::SetCamera(Camera *cam)
{
	Planet::cam = cam;
}

void Planet::SetShadowCamera(Camera *shadowCam)
{
	Planet::shadowCam = shadowCam;
}

void Planet::SetShadowTexture(int shadowTextureNum)
{
	Planet::shadowTextureNum = shadowTextureNum;
}

void Planet::SetPlayerPos(Vector3 playerPos)
{
	sPlayerPos = playerPos;
}

void Planet::GrabOn()
{
	isGrab_ = true;
}

void Planet::ReleaseGrab()
{
	isGrab_ = false;
}

void Planet::OnPlayer()
{
	isOnPlayer_ = true;
}

void Planet::ReleaseBase()
{
	isOnPlayer_ = false;
}

void Planet::SetGrabRotateAngle(const DirectX::XMVECTOR &AxisY, const DirectX::XMVECTOR &AxisX)
{
	GrabRotateAxisY = AxisY;
	GrabRotateAxisX = AxisX;
}

void Planet::SetNextOrder(bool isNext)
{
	isNext_ = isNext;
	isNextLightup_ = true;
}

bool Planet::GetIsOnPlayer()
{
	return isOnPlayer_;
}

int Planet::GetID()
{
	return ID_;
}

bool Planet::GetIsNext()
{
	return isNext_;
}

void Planet::SetIsBloom(bool isBloom)
{
	isBloom_ = isBloom;
}

bool Planet::GetIsBloom()
{
	return isBloom_;
}

PlanetType Planet::GetType()
{
	return type_;
}

void Planet::Reset()
{
	if (startIsSpawn_)
	{
		SetScale(startScale_);
		isSpawn_ = true;
	}
	else
	{
		SetScale(0.0f);
		isSpawn_ = false;
		isSpawnAnimation_ = false;
	}
	pos = startPos_;
	isNextLightup_ = true;

	clearRate = 0.0f;
	isBloom_ = false;
	isColorChange_ = false;
	object->SetPosition(startPos_);
	object->SetColor(startColor_);
}
