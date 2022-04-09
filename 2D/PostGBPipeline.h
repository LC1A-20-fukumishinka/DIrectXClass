#pragma once
#include "IGraphicsPipeline.h"
class PostGBPipeline :
	public IGraphicsPipeline
{
private:
	PostGBPipeline();
	~PostGBPipeline();
public:

	const PipeClass::PipelineSet &GetPipeLine();

	static PostGBPipeline *Instance();
private:
	PipeClass::PipelineSet pipelineSet;
};

