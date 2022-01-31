#pragma once

#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "Sprite.h"
#include "Framework.h"

class GameScene;
class Game : public Framework
{
public:
	void Init() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
public:
private:

		Sound *alarm;
		Camera *cam;
		float angle;	//ƒJƒƒ‰‚Ì‰ñ“]Šp
		Object3D *boxObj;

		Model *triangle;
		ParticleManager *part;
		int spriteTex;
		Sprite *sprite;

		GameScene *scene_ = nullptr;
};