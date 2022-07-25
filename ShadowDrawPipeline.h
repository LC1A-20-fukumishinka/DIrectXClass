#pragma once
#include "IGraphicsPipeline.h"
class ShadowDrawPipeline
{
	ShadowDrawPipeline();
	~ShadowDrawPipeline();
public:

	PipeClass::PipelineSet *GetPipeLine();

	static ShadowDrawPipeline *Instance();
private:
	std::unique_ptr<PipeClass::PipelineSet> pipelineSet;
};

