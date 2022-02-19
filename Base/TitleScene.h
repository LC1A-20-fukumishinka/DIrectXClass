#pragma once
#include "Sprite.h"
#include "Object3D.h"
#include "Model.h"
#include "../Camera.h"
#include "../Sound.h"
#include "particleManager.h"
#include "DebugText.h"
#include "IScene.h"
#include "../Light.h"
#include "../LightGroup.h"
class TitleScene : public IScene
{
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
private:
	Camera *cam;
	float angle;	//ƒJƒƒ‰‚Ì‰ñ“]Šp
	Object3D *boxObj;

	Model *triangle;
	ParticleManager *part;
	int spriteTex;
	Sprite *sprite;
	Sound *alarm;

	DebugText *debug;
	Light *light;
	LightGroup *lightGroup = nullptr;
};