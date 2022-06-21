#pragma once
#include "../Collision/BaseCollider.h"
#include "../Collision/CollisionPrimitive.h"
#include <DirectXMath.h>

class MeshCollider : public BaseCollider
{
public:
	MeshCollider()
	{
		//メッシュ形状をセット
		shapeType = COLLISIONSHAPE_MESH;
	}

	void ConstructTraiangles(Model *model);


	void Update() override;

	bool CheckCollisionSphere(const Sphere &sphere, DirectX::XMVECTOR *inter = nullptr);

	bool CheclCollisionRay(const Ray &ray, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

private:
	std::vector<Triangle> triangles;

	DirectX::XMMATRIX invMatWorld;
};