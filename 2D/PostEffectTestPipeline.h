#pragma once
#include "IGraphicsPipeline.h"
class PostEffectTestPipeline :
	public IGraphicsPipeline
{
private:
	PostEffectTestPipeline();
	~PostEffectTestPipeline();
public:

	PipeClass::PipelineSet *GetPipeLine();

	static PostEffectTestPipeline *Instance();
private:
	std::unique_ptr <PipeClass::PipelineSet> pipelineSet;
};

