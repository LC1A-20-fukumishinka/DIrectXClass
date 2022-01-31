#pragma once
#include "Sprite.h"
#include "Object3D.h"
#include "Model.h"
#include "../Camera.h"
class GameScene
{
public:
	void Init();
	void Update();
	void Draw();
	void Finalize();
private:
	Model *dome;
	Object3D *domeObj;
	Camera *cam;


};