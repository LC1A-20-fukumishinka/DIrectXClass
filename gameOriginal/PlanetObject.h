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

	struct PlanetAnimationData
	{
		DirectX::XMFLOAT3 planetToPlayerAngle;
		float whiteRate = 0.0f;
		DirectX::XMFLOAT3 planetPos;
		float colorRate = 0.0f;
	};
public:
	void Init() override;
	void Update() override;

	void modelDraw(PipeClass::PipelineSet *pipelineSet, bool isSetTexture = false, int textureNumber = -1) override;

	void SendPlanetAnimationData(const Vector3 &PlanetCenterPos, float whiteRate, float colorRate);
public:
	void SetShadowCamera(Camera *cam);
	void SetShadowTextureNum(int shadowTextureNum);
private:
	Camera *shadowCamera = nullptr;
	int shadowTexture = -1;

	Microsoft::WRL::ComPtr<ID3D12Resource> planetAnimationDataBuff;
};

