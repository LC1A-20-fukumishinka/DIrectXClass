#pragma once
#include "Object3D.h"
#include "Model.h"
#include "Vector3.h"
#include <memory>
class Camera;
class LightGroup;
class Planet
{
public:
	Planet();
	~Planet();

	void Init(const DirectX::XMFLOAT3 &pos, float size, const DirectX::XMFLOAT4 &color);
	void Update();
	void NormalUpdate();
	void GrabUpdate();
	void Draw();
	void Finalize();
	const Vector3 &GetPos(){return pos;}
	void SetPos(const DirectX::XMFLOAT3 &pos){this->pos = Vector3(pos.x, pos.y, pos.z);}
	void SetScale(float scale);
	float GetScale();
	std::unique_ptr<Object3D> object;
	std::unique_ptr <Model> model;
public:
	static void SetLight(LightGroup *lights);
	static void SetCamera(Camera *cam);
	void GrabInput();
	void ReleaseGrab();
	void OnPlayer();
	void ReleaseBase();
	void SetGrabRotateAngle(const DirectX::XMVECTOR AxisY, const DirectX::XMVECTOR AxisX);
	bool GetBase();
private://Ã“Iƒƒ“ƒo•Ï”
	static Camera *cam;
	static LightGroup *lights;
private://ƒƒ“ƒo•Ï”

	Vector3 pos;

	bool isGrab = false;
	bool isBase = false;
	DirectX::XMVECTOR GrabRotateAxisY = {};
	DirectX::XMVECTOR GrabRotateAxisX = {};
	float scale = 3.0f;
};

