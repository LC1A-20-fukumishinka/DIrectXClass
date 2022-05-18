#pragma once
#include "Sprite.h"
#include "Object3D.h"
#include "Model.h"
#include "../Camera.h"
#include "IScene.h"
#include "../Light.h"
#include "SphereCollider.h"
#include "../Collision/CollisionMgr.h"
#include "Player.h"
#include "particleManager.h"
#include "../LightGroup.h"
#include "../PointLight.h"
#include "../SpotLight.h"
#include "FbxModel.h"
#include "FbxObject3D.h"
#include "../Planet.h"
class TouchableObject;
class GameScene : public IScene
{
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
private:
	std::unique_ptr<Planet> star;

	std::unique_ptr<Camera> cam;

	std::unique_ptr<Light> light;
	std::unique_ptr<PointLight> pointLight;
	std::unique_ptr<SpotLight> spotLight;
	std::unique_ptr<LightGroup> lightGroup;

	std::unique_ptr<Model> groundModel;
	std::unique_ptr<Object3D> objGround;
	std::unique_ptr<FbxModel> templeModel;
	std::unique_ptr<FbxObject3D> temple;
};