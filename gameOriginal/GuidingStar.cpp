#include "GuidingStar.h"
#include "ModelPhongPipeline.h"
#include "../EaseClass.h"
#include <algorithm>
std::unique_ptr<Model> GuidingStar::sModel;
Camera *GuidingStar::sCamera = nullptr;
LightGroup *GuidingStar::sLightGroup = nullptr;
std::unique_ptr<PipeClass::PipelineSet> GuidingStar::sPipeline;
GuidingStar::GuidingStar()
{
	if (!sModel)
	{
		sModel = std::make_unique<Model>();
		sModel->CreateModel("box");
	}
}

GuidingStar::~GuidingStar()
{
}

void GuidingStar::Create()
{
	starObject_.Init();
	starObject_.SetCamera(sCamera);
	starObject_.SetLightGroup(sLightGroup);
	starObject_.SetModel(sModel.get());
	SetScale(3.0f);
}

void GuidingStar::Init(Vector3 destination, Vector3 startPos)
{
	destination_ = destination;
	startPos_ = startPos;
	drawPos_ = startPos_;
	isAlive_ = true;
	moveRate_ = 0.0f;
}

void GuidingStar::Update()
{
	//二点間の距離(最初に一回だけでよさそう)
	Vector3 moveDistance = destination_ - startPos_;

	//移動速度の計算(最初に一回だけでよさそう)
	moveRate_ += (1.0f / 300.0f);
	moveRate_ = std::clamp(moveRate_, 0.0f, 1.0f);
	float easeRate = Easing::easeOutExpo(moveRate_);

	easeRate *= 1.1f;
	drawPos_ = startPos_ + (moveDistance * easeRate);

	//確認用
	if (easeRate >= 1.0f)
	{
		isAlive_ = false;
	}
}

void GuidingStar::Draw()
{
	if (!isAlive_) return;
	starObject_.SetPosition(drawPos_);
	starObject_.Update();
	starObject_.modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
}

void GuidingStar::Finalize()
{
}

void GuidingStar::SetScale(float scale)
{
	starObject_.SetScale(Vector3(scale, scale, scale));
}

bool GuidingStar::GetIsDraw()
{
	return isAlive_;
}

void GuidingStar::SetCamera(Camera *camera)
{
	sCamera = camera;
}

void GuidingStar::SetLightGroup(LightGroup *lightGroup)
{
	sLightGroup = lightGroup;
}
