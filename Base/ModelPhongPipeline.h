#pragma once
#include "IGraphicsPipeline.h"

class ModelPhongPipeline : public IGraphicsPipeline
{
private:
	ModelPhongPipeline();
	~ModelPhongPipeline();
public:

	PipeClass::PipelineSet *GetPipeLine();

	static ModelPhongPipeline *Instance();
private:
	std::unique_ptr<PipeClass::PipelineSet> pipelineSet;
};