#pragma once
#include "IGraphicsPipeline.h"

class GraphicsPipeline3D : public IGraphicsPipeline
{
private:
	GraphicsPipeline3D();
	~GraphicsPipeline3D();
public:

	const PipeClass::PipelineSet &GetPipeLine();

	static GraphicsPipeline3D *Instance();
private:
	PipeClass::PipelineSet pipelineSet;
};