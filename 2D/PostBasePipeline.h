#pragma once
#include "IGraphicsPipeline.h"
/// <summary>
/// 特に何も処理をせず描画する
/// </summary>
class PostBasePipeline :
	public IGraphicsPipeline
{
public:
	PostBasePipeline();
	~PostBasePipeline();
	PipeClass::PipelineSet *GetPipeLine() override;
private:
	std::unique_ptr<PipeClass::PipelineSet> pipelineSet;
};

//結局シェーダーをオブジェクトによって書き分けたりするようになると考えるとシングルトンで作るとかウンチじゃん