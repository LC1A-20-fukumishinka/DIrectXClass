#pragma once
#include "WinAPI.h"
#include "MyDirectX.h"
#include "Sound.h"
#include "Model.h"
#include "3D/Object3D.h"
#include "Camera.h"
#include "particleManager.h"
#include "2D/Sprite.h"
#include "TextureMgr.h"

class Game
{
public:
	void Init();
	void Finalize();
	void Update();
	void PreDraw();
	void Draw();
	void PostDraw();
public:
bool isEnd;
private:
	WinAPI *Win;
		MyDirectX *myDirectX;

		Sound alarm;
		Camera cam;
		float angle;	//ÉJÉÅÉâÇÃâÒì]äp
		Object3D domeObj;
		Object3D boxObj;

		Model triangle;
		Model dome;
		ParticleManager *part;
		int spriteTex;
		Sprite sprite;
};