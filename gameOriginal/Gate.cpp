#include "Gate.h"
#include <DirectXMath.h>
#include "../Collision/Collision.h"
#include "ModelPhongPipeline.h"
Model* Gate::sModel = nullptr;
Camera* Gate::sCamera = nullptr;
LightGroup* Gate::sLights = nullptr;
Gate::Gate()
{
}

Gate::~Gate()
{
}

void Gate::Init(const Vector3& pos, const Vector3& angle, int index, bool isDraw)
{
	pos_ = pos;
	angle_ = angle;
	isDraw_ = isDraw;
	index_ = index;
	object_.Init();
	object_.SetCamera(sCamera);
	object_.SetLightGroup(sLights);
	object_.SetModel(sModel);
	Reset();
}

void Gate::Update()
{
}

void Gate::Draw()
{
	if (isAlive_)
	{
		object_.Update();
		object_.modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
	}
}

void Gate::Finalize()
{
}

void Gate::Reset()
{
	object_.SetScale(Vector3(scale_, scale_, 0.1f));
	object_.SetPosition(pos_);
	object_.SetRotationVector(F3toV(angle_));
	object_.Update();
	isAlive_ = true;
}

const DirectX::XMFLOAT3& Gate::GetPos()
{
	return object_.GetPosition();
}

Vector3 Gate::GetAngle()
{
	Vector3 angle = DirectX::XMVector3Rotate(FukuMath::ZVec, object_.GetRotQuaternion());
	return angle;
}

void Gate::SetModel(Model* model)
{
	sModel = model;
}

void Gate::SetCamera(Camera* camera)
{
	sCamera = camera;
}
void Gate::SetLightGroup(LightGroup* lights)
{
	sLights = lights;
}

bool Gate::Collision(const Sphere& player)
{
	if (!isAlive_)
	{
		return false;
	}
	Sphere gateSphere;
	gateSphere.center = F3toV(pos_);
	gateSphere.radius = scale_;

	if (Collision::CheckSphere2Sphere(player, gateSphere))
	{
		Plane gatePlane;
		gatePlane.pos = F3toV(pos_);
		gatePlane.normal = F3toV(angle_);

		//isAlive_ = false;
		if (Collision::CheckSphere2Plane(player, gatePlane))
		{
			isAlive_ = false;
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}
