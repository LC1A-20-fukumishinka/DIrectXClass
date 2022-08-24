#pragma once
#include "ObjectOnSphere.h"
#include <vector>
#include "../Collision/CollisionPrimitive.h"
class Block : public ObjectOnSphere
{
public:
	Block();
	~Block();
	void Init(std::weak_ptr<Planet> basePlanet, Vector3 angle, float scale) override;
	//void Update() override;

	std::vector<Triangle> GetCollisionPlanes();
	
private:
	static std::unique_ptr<Model> sBlockModel;

public:

	//void CollisionPlayer(float radius, const Vector3 pos, const Vector3 up);


};

