#pragma once

#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "Sprite.h"
#include "Framework.h"
class Game : public Framework
{
public:
	void Init() override;
	void Finalize();
	void Update();
	void PreDraw();
	void Draw();
	void PostDraw();
public:
bool isEnd;
private:

		Sound alarm;
		Camera cam;
		float angle;	//ƒJƒƒ‰‚Ì‰ñ“]Šp
		Object3D *domeObj;
		Object3D *boxObj;

		Model triangle;
		Model dome;
		ParticleManager *part;
		int spriteTex;
		Sprite *sprite;
};