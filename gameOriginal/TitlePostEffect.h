#pragma once
#include "PostEffect.h"
#include "IGraphicsPipeline.h"
#include <memory>
#include "../EaseClass.h"
class TitlePostEffect
{
	struct TitleBuffer
	{
		float t;
		int type;
	};
public:
	TitlePostEffect();
	~TitlePostEffect();
	void Init();
	void Update();
	void Draw(int textureHandle);
	void Finalize();
private:

	std::unique_ptr<PipeClass::PipelineSet> pipeline;


	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff2_;
	float t = 0.5f;
	int Titletype = 0;
	PostEffect obj;
	Easing animationTimer;
};

