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
	upVec_ = angle;//�I�u�W�F�N�g�̕�����ݒ�
	worldScale_ = scale;
	upVec_.normalize();//�ꉞ���K��
	frontVec_ = Vector3(upVec_.y, -upVec_.x, 0.0f).cross(upVec_);
	basePlanet_ = basePlanet;
	object_.Init();

	object_.SetCamera(sMainCamera);
	object_.SetLightGroup(sLights);

	RotationUpdate();

	if (!basePlanet_.expired())
	{
		//�f���̒��S����
		Vector3 pos = basePlanet_.lock()->GetPos();
		//�w�肵������(upVec)�ɘf���̔��a���ړ��������ꏊ��
		pos += (upVec_ * basePlanet_.lock()->GetStartScale());
		//���W�ɂȂ�
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
	//��ŃI���W�i���̃V�F�[�_�[�����

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
//	//���݈ʒu����
//	Vector3 pos = GetWorldPos();
//
//	//�ړ��ʂ����Z���Ĉړ���̈ʒu�����
//	pos += MoveVec;
//
//	//���݈ʒu - �f���̈ʒu = �f������̑��Έʒu
//	pos -= basePlanet_.lock()->GetPos();
//
//	//���Έʒu����f������̕������擾(���łɏ�����C��)
//	upVec_ = pos.normalize();
//	//�����Ƙf���̔��a���g���Ęf���\�ʂ̍��W�ɕ␳
//	pos = pos.normalize() * basePlanet_.lock()->GetScale();
//
//	//�f���̍��W�𑫂��đ��΍��W���烏�[���h���W�ɂ���
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
