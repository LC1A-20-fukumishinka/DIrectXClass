#pragma once
#include "Framework.h"
#include "PostEffectTestPipeline.h"
#include "PostMosaicPipeline.h"
#include "PostMonochromePipeline.h"
#include "PostGBPipeline.h"
#include "PostEffect.h"
class Game : public Framework
{
public:
	void Init() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
public:
private:
	std::unique_ptr<PostEffect> postTest;
};