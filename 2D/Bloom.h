#pragma once
#include "PostEffect.h"
#include "IGraphicsPipeline.h"
#include <memory>
#include "../EaseClass.h"
#include "MultiRenderTarget.h"
class Bloom
{
	struct TitleBuffer
	{
		float t;
		int type;
	};
public:
	Bloom();
	~Bloom();
	void Init();
	void Update();
	void BrightUpdate(int brightTextureHandle);
	void Draw(int mainTextureHandle);
	void Finalize();
private:

	std::unique_ptr<PipeClass::PipelineSet> bloomPipeline;
	PostEffect brightnessExtractionObj;

	std::unique_ptr<PipeClass::PipelineSet> GaussianBlurPipeline;
	PostEffect bloomObj;
	std::vector<MultiRenderTarget> bloomTargets;
};

