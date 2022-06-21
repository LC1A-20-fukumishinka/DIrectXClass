#pragma once
#include "IGraphicsPipeline.h"

class ModelPipelineAdd : public IGraphicsPipeline
{
private:
	ModelPipelineAdd();
	~ModelPipelineAdd();
public:

	PipeClass::PipelineSet *GetPipeLine();

	static ModelPipelineAdd *Instance();
private:
	std::unique_ptr<PipeClass::PipelineSet> pipelineSet;
};