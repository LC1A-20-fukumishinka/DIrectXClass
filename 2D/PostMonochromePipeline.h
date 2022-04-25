#pragma once
#include "IGraphicsPipeline.h"
class PostMonochromePipeline :
	public IGraphicsPipeline
{
private:
	PostMonochromePipeline();
	~PostMonochromePipeline();
public:

	PipeClass::PipelineSet *GetPipeLine();

	static PostMonochromePipeline *Instance();
private:
	std::unique_ptr<PipeClass::PipelineSet> pipelineSet;
};

