#pragma once
#include "IGraphicsPipeline.h"
class PostGBPipeline :
	public IGraphicsPipeline
{
private:
	PostGBPipeline();
	~PostGBPipeline();
public:

	PipeClass::PipelineSet *GetPipeLine();

	static PostGBPipeline *Instance();
private:
	std::unique_ptr<PipeClass::PipelineSet> pipelineSet;
};

