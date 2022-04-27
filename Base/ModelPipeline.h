#pragma once
#include "IGraphicsPipeline.h"

class ModelPipeline : public IGraphicsPipeline
{
private:
	ModelPipeline();
	~ModelPipeline();
public:

	PipeClass::PipelineSet *GetPipeLine();

	static ModelPipeline *Instance();
private:
	std::unique_ptr<PipeClass::PipelineSet> pipelineSet;
};