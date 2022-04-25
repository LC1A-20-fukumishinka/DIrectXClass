#pragma once
#include "IGraphicsPipeline.h"
#include <memory>
class GraphicsPipeline3D : public IGraphicsPipeline
{
private:
	GraphicsPipeline3D();
	~GraphicsPipeline3D();
public:

	PipeClass::PipelineSet *GetPipeLine();

	static GraphicsPipeline3D *Instance();
private:
	std::unique_ptr<PipeClass::PipelineSet> pipelineSet;
};