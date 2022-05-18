#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "Object3D.h"
#include "Model.h"
#include "../Camera.h"
#include "../Light.h"
#include "../LightGroup.h"
#include "../PointLight.h"
#include "../SpotLight.h"
#include <memory>
#include "Vector3.h"
class GameScene : public IScene
{
private:
	template <class T>
	using unique_ptr = std::unique_ptr<T>;
public://基本関数
	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
public://内部的に使用する関数
	Vector3 AirResistance(const Vector3 &velocity, const float k);
private:
	unique_ptr<Model> dome;
	unique_ptr<Model> fighterModel;
	unique_ptr<Model> groundModel;
	unique_ptr<Object3D> domeObj;
	unique_ptr<Object3D> SphereA;
	unique_ptr<Object3D> SphereB;
	unique_ptr<Object3D> objectSphere;
	unique_ptr<Camera> cam;
	unique_ptr<Light> light;
	unique_ptr<PointLight> pointLight;
	unique_ptr<SpotLight> spotLight;
	unique_ptr<LightGroup> lightGroup;
	int partTex;

	unique_ptr <Object3D> objGround;

	float power;
	float gravity;
	float airResistance;
	float friction;
	Vector3 startAngle;
	Vector3 jumpVector;

	bool isStart = false;
};