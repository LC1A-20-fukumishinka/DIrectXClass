#pragma once
#include "IGraphicsPipeline.h"

class ModelPhongPipeline : public IGraphicsPipeline
{
private:
	ModelPhongPipeline();
	~ModelPhongPipeline();
public:

	const PipeClass::PipelineSet &GetPipeLine();

	static ModelPhongPipeline *Instance();
private:
	PipeClass::PipelineSet pipelineSet;
};