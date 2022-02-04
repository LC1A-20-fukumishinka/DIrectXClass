#pragma once
#include "IGraphicsPipeline.h"

class ModelPipeline : public IGraphicsPipeline
{
private:
	ModelPipeline();
	~ModelPipeline();
public:

	const PipeClass::PipelineSet &GetPipeLine();

	static ModelPipeline *Instance();
private:
	PipeClass::PipelineSet pipelineSet;
};