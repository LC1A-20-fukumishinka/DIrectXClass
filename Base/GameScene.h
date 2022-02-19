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
class TouchableObject;
class GameScene : public IScene
{
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
private:
	Model *dome;
	Model *sphereModel;
	Model *fighterModel;
	Model *groundModel;
	Object3D *domeObj;
	Object3D *objFighter;
	Object3D *objectSphere;
	Camera *cam;

	Light *light = nullptr;
	LightGroup *lightGroup = nullptr;
	int partTex;
	ParticleManager *rayPart = nullptr;

	TouchableObject *objGround = nullptr;
};