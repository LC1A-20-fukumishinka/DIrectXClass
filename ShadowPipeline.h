#pragma once
#include "IGraphicsPipeline.h"
class ShadowPipeline : public IGraphicsPipeline
{
private:
	ShadowPipeline();
	~ShadowPipeline();
public:

	PipeClass::PipelineSet *GetPipeLine();

	static ShadowPipeline *Instance();
private:
	std::unique_ptr<PipeClass::PipelineSet> pipelineSet;
};
