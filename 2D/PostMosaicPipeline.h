#pragma once
#include "IGraphicsPipeline.h"
class PostMosaicPipeline :
	public IGraphicsPipeline
{
private:
	PostMosaicPipeline();
	~PostMosaicPipeline();
public:

	PipeClass::PipelineSet *GetPipeLine();

	static PostMosaicPipeline *Instance();
private:
	std::unique_ptr<PipeClass::PipelineSet> pipelineSet;
};

