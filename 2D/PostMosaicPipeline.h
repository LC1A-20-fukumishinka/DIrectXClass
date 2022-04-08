#pragma once
#include "IGraphicsPipeline.h"
class PostMosaicPipeline :
	public IGraphicsPipeline
{
private:
	PostMosaicPipeline();
	~PostMosaicPipeline();
public:

	const PipeClass::PipelineSet &GetPipeLine();

	static PostMosaicPipeline *Instance();
private:
	PipeClass::PipelineSet pipelineSet;
};

