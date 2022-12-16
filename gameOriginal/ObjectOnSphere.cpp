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

void ObjectOnSphere::Init(std::weak_ptr<Planet> basePlanet,const Vector3 &angle, float scale)
{
	upVec_ = angle;//オブジェクトの方向を設定
	worldScale_ = scale;
	upVec_.normalize();//一応正規化
	frontVec_ = Vector3(upVec_.y, -upVec_.x, 0.0f).cross(upVec_);
	basePlanet_ = basePlanet;
	object_.Init();

	object_.SetCamera(sMainCamera);
	object_.SetLightGroup(sLights);

	RotationUpdate();

	if (!basePlanet_.expired())
	{
		//惑星の中心から
		Vector3 pos = basePlanet_.lock()->GetPos();
		//指定した方向(upVec)に惑星の半径分移動させた場所が
		pos += (upVec_ * basePlanet_.lock()->GetStartScale());
		//座標になる
		object_.SetPosition(pos);
		Vector3 identity = { 1.0f,1.0f ,1.0f };
		object_.SetScale(identity * (worldScale_));
	}
}

void ObjectOnSphere::Update()
{
	object_.SetCamera(sMainCamera);
	object_.SetLightGroup(sLights);
	object_.SetRotationVector(F3toV(frontVec_), F3toV(upVec_));
	RotationUpdate();
}

void ObjectOnSphere::Draw()
{
	object_.Update();
	//後でオリジナルのシェーダーを作れ

	if (!basePlanet_.lock()->GetIsSpawn()) return;
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

DirectX::XMVECTOR ObjectOnSphere::GetRotationQ()
{
	return object_.GetRotQuaternion();
}

bool ObjectOnSphere::GetIsPlanetSpawn()
{
	return basePlanet_.lock()->GetIsSpawn();
}

//void ObjectOnSphere::Move(Vector3 MoveVec)
//{
//	//現在位置から
//	Vector3 pos = GetWorldPos();
//
//	//移動量を加算して移動後の位置を作る
//	pos += MoveVec;
//
//	//現在位置 - 惑星の位置 = 惑星からの相対位置
//	pos -= basePlanet_.lock()->GetPos();
//
//	//相対位置から惑星からの方向を取得(ついでに上方向修正)
//	upVec_ = pos.normalize();
//	//方向と惑星の半径を使って惑星表面の座標に補正
//	pos = pos.normalize() * basePlanet_.lock()->GetScale();
//
//	//惑星の座標を足して相対座標からワールド座標にする
//	pos += basePlanet_.lock()->GetPos();
//	object_.SetPosition(pos);
//
//
//	upVec_.normalize();
//	pos += (upVec_ * basePlanet_.lock()->GetScale());
//}

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
	XMVECTOR frontVecV = F3toV(frontVec_);
	XMVECTOR upVecV = F3toV(upVec_);
	if (XMVector3Equal(frontVecV, upVecV))
	{
		frontVec_ = Vector3(0.0f, -1.0f, 0.0f);

		frontVecV = F3toV(frontVec_);
	}
	object_.SetRotationVector(frontVecV, upVecV);
}
