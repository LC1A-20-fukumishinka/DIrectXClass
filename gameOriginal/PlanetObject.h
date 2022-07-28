#pragma once
#include "Object3D.h"

class PlanetObject : public Object3D
{
	struct PlanetConstBufferData
	{
		DirectX::XMFLOAT4 color;	//êF(RGBA)
		DirectX::XMMATRIX viewproj;
		DirectX::XMMATRIX shadowViewproj;
		DirectX::XMMATRIX world;
		DirectX::XMFLOAT3 cameraPos;
	};

public:
	void Init() override;
	void Update() override;

	void modelDraw(PipeClass::PipelineSet *pipelineSet, bool isSetTexture = false, int textureNumber = -1) override;

public:
	void SetShadowCamera(Camera *cam);
	void SetShadowTextureNum(int shadowTextureNum);
private:
	Camera *shadowCamera = nullptr;
	int shadowTexture = -1;
};

