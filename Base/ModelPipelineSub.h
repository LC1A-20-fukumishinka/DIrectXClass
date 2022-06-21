#pragma once
#include "IGraphicsPipeline.h"

class ModelPipelineSub : public IGraphicsPipeline
{
private:
	ModelPipelineSub();
	~ModelPipelineSub();
public:

	PipeClass::PipelineSet *GetPipeLine();

	static ModelPipelineSub *Instance();
private:
	std::unique_ptr<PipeClass::PipelineSet> pipelineSet;
};