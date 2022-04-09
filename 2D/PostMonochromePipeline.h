#pragma once
#include "IGraphicsPipeline.h"
class PostMonochromePipeline :
	public IGraphicsPipeline
{
private:
	PostMonochromePipeline();
	~PostMonochromePipeline();
public:

	const PipeClass::PipelineSet &GetPipeLine();

	static PostMonochromePipeline *Instance();
private:
	PipeClass::PipelineSet pipelineSet;
};

