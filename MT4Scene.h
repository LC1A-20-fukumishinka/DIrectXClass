#pragma once
#include "IScene.h"
class MT4Scene : public IScene
{
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
private:
};

