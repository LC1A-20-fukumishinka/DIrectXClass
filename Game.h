#pragma once
#include "Framework.h"
class Game : public Framework
{
public:
	void Init() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
public:
private:
};