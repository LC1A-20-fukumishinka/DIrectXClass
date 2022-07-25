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
	void PreDraw() override;
	void MainDraw() override;
	void Finalize() override;
private:


	DebugText *debug;

};