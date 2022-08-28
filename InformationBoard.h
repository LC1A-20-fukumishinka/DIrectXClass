#pragma once
#include "Object3D.h"
#include "Model.h"
#include <memory>
#include "Camera.h"
#include "../LightGroup.h"
#include "IGraphicsPipeline.h"
class InformationBoard
{
public:
	InformationBoard();
	~InformationBoard();

	void Init(const wchar_t *filename, const Vector3 &basePos, const Vector3 &movePos, const Vector3 &scale);
	void Update();
	void Draw();
	void Finalize();

	bool CollisionPlayer(const Vector3 &pos);
private:
	static std::unique_ptr<Model> sModel;
	static Camera *sCamera;
	static LightGroup *sLightGroup;
	static std::unique_ptr<PipeClass::PipelineSet> sPipeline;

public:
	static void SetLightGroup(LightGroup *lightGroup);
	static void SetCamera(Camera *camera);
private:
	Object3D drawObject_;
	int textureHandle_;
	Vector3 movePos_;
	Vector3 basePos_;

	Vector3 baseScale_;
	bool isInDrawLength_ = false;
	float objectScale_ =false;
	float drawLength_ = 10.0f;
};