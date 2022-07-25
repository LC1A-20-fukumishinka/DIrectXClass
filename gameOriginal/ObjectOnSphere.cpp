#include "ObjectOnSphere.h"
#include "ModelPhongPipeline.h"
#include "Model.h"
#include "Planet.h"
using namespace DirectX;
Camera *ObjectOnSphere::sMainCamera = nullptr;
LightGroup *ObjectOnSphere::sLights = nullptr;

ObjectOnSphere::ObjectOnSphere()
{
}

ObjectOnSphere::~ObjectOnSphere()
{
}

void ObjectOnSphere::Init(std::weak_ptr<Planet> basePlanet, Vector3 angle, float scale)
{
	upVec_ = angle;
	worldScale_ = scale;
	upVec_.normalize();

	basePlanet_ = basePlanet;
	object_.Init();

	object_.SetCamera(sMainCamera);
	object_.SetLightGroup(sLights);

	RotationUpdate();

	if (!basePlanet_.expired())
	{
		Vector3 pos = {};

		object_.SetParent(basePlanet_.lock()->GetObject3d());
		pos += (upVec_ * 1.0f);
		object_.SetPosition(pos);
		Vector3 identity = { 1.0f,1.0f ,1.0f };
		object_.SetScale(identity * (worldScale_ / basePlanet_.lock()->GetScale()));
	}
}

void ObjectOnSphere::Update()
{
	object_.SetCamera(sMainCamera);
	object_.SetLightGroup(sLights);
	object_.SetRotationVector(XMLoadFloat3(&frontVec_), XMLoadFloat3(&upVec_));
	RotationUpdate();
}

void ObjectOnSphere::Draw()
{
	object_.Update();
	//後でオリジナルのシェーダーを作れ
	object_.modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
}

void ObjectOnSphere::Finalize()
{
}

void ObjectOnSphere::SetModel(Model *model)
{
	object_.SetModel(model);
}

Vector3 ObjectOnSphere::GetWorldPos()
{
	return object_.GetWorldPos();
}

float ObjectOnSphere::GetWorldScale()
{
	return worldScale_;
}

void ObjectOnSphere::SetCamera(Camera *camera)
{
	sMainCamera = camera;
}


void ObjectOnSphere::SetLights(LightGroup *Lights)
{
	sLights = Lights;
}

void ObjectOnSphere::RotationUpdate()
{
	XMVECTOR frontVecV = XMLoadFloat3(&frontVec_);
	XMVECTOR upVecV = XMLoadFloat3(&upVec_);
	if (XMVector3Equal(frontVecV, upVecV))
	{
		frontVec_ = Vector3(0.0f, -1.0f, 0.0f);

		frontVecV = XMLoadFloat3(&frontVec_);
	}
	object_.SetRotationVector(frontVecV, upVecV);
}
