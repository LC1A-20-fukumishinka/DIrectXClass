#pragma once
#include "IGraphicsPipeline.h"
class PostEffectTestPipeline :
	public IGraphicsPipeline
{
private:
	PostEffectTestPipeline();
	~PostEffectTestPipeline();
public:

	const PipeClass::PipelineSet &GetPipeLine();

	static PostEffectTestPipeline *Instance();
private:
	PipeClass::PipelineSet pipelineSet;
};

