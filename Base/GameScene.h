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
	Object3D *domeObj;
	Object3D *objFighter;
	Object3D *objectSphere;
	Camera *cam;

	Light *light;
	int partTex;
	ParticleManager *rayPart;

};