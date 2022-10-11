#pragma once
#include "Object3D.h"
#include "Model.h"
#include "../Camera.h"
#include "../LightGroup.h"
#include "IGraphicsPipeline.h"
#include <memory>
class GuidingStar
{
public:
	GuidingStar();
	~GuidingStar();

	//ê∂ê¨ÇµÇΩç≈èâÇ…åƒÇ—èoÇ∑
	void Create();

	void Init(Vector3 destination, Vector3 startPos);
	void Update();
	void Draw();
	void Finalize();

	void SetScale(float scale);

	bool GetIsDraw();

private:
	static std::unique_ptr<Model> sModel;
	static Camera *sCamera;
	static LightGroup *sLightGroup;
	static std::unique_ptr<PipeClass::PipelineSet> sPipeline;
public:
static void SetCamera(Camera *camera);
static void SetLightGroup(LightGroup *lightGroup);
private:
	Vector3 startPos_;
	Vector3 destination_;
	Vector3 drawPos_;
	float power_ = 1.0f;
	Object3D starObject_;

	bool isAlive_ = true;
	float moveRate_ = 0.0f;
};

