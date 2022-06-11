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

	void Init();
	void Update();
	void NormalUpdate();
	void GrabUpdate();
	void Draw();
	void Finalize();
	const Vector3 &GetPos(){return pos;}
	void SetPos(const DirectX::XMFLOAT3 &pos){this->pos = Vector3(pos.x, pos.y, pos.z);}
	std::unique_ptr<Object3D> object;
	std::unique_ptr <Model> model;
public:
	static void SetLight(LightGroup *lights);
	static void SetCamera(Camera *cam);
	void Grab();
	void ReleaseGrab();
	void SetGrabRotateAngle(DirectX::XMVECTOR Axis);
private://Ã“Iƒƒ“ƒo•Ï”
	static Camera *cam;
	static LightGroup *lights;
private://ƒƒ“ƒo•Ï”

	Vector3 pos;

	bool isGrab = false;
	DirectX::XMVECTOR GrabRotateAxis = {};
};

