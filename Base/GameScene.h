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
class GameScene : public IScene
{
private:
	template <class T>
	using unique_ptr =std::unique_ptr<T>;
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
private:
	unique_ptr<Model> dome;
	unique_ptr<Model> fighterModel;
	unique_ptr<Model> groundModel;
	unique_ptr<Object3D> domeObj;
	unique_ptr<Object3D> objFighter;
	unique_ptr<Object3D> objectSphere;
	unique_ptr<Camera> cam;
	unique_ptr<Light> light;
	unique_ptr<PointLight> pointLight;
	unique_ptr<SpotLight> spotLight;
	unique_ptr<LightGroup> lightGroup;
	int partTex;

	unique_ptr < Object3D> objGround;

	float power;
	float gravity;
	Vector3 startAngle;
	Vector3 jumpVector;

	bool isStart = false;
};