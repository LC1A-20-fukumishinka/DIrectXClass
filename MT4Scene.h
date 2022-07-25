#pragma once
#include "IScene.h"
class MT4Scene : public IScene
{
public:
	void Init() override;
	void Update() override;
	void PreDraw() override;
	void MainDraw() override;
	void Finalize() override;
private:
};

