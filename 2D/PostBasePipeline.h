#pragma once
#include "IGraphicsPipeline.h"
/// <summary>
/// ���ɉ��������������`�悷��
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

//���ǃV�F�[�_�[���I�u�W�F�N�g�ɂ���ď����������肷��悤�ɂȂ�ƍl����ƃV���O���g���ō��Ƃ��E���`�����