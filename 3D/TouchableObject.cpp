#include "TouchableObject.h"
#include "MeshCollider.h"
TouchableObject *TouchableObject::create(Model *model)
{
	TouchableObject *instance = new TouchableObject();
	if (instance == nullptr)
	{
		return nullptr;
	}

	if (!instance->Initialize(model))
	{
		delete instance;
		assert(0);
	}
	return instance;
}

bool TouchableObject::Initialize(Model *model)
{
	Object3D::Init();

	SetModel(model);

	MeshCollider *collider = new MeshCollider;
	SetCollider(collider);
	collider->ConstructTraiangles(model);
	return true;
}
