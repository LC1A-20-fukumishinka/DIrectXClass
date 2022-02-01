#pragma once
#include "IGraphicsPipeline.h"

class ModelPhongPipeline : public IGraphicsPipeline
{
private:
	ModelPhongPipeline();
	~ModelPhongPipeline();
public:

	const PipeClass::PipelineSet &GetPipeLine();

	static ModelPhongPipeline *GetInstance();
private:
	PipeClass::PipelineSet pipelineSet;
};