#pragma once
#include "PlanetObject.h"
#include "Model.h"
#include "Vector3.h"
#include <memory>
#include "../EaseClass.h"
class Camera;
class LightGroup;

const enum PlanetType
{
	NONE,
	BASE,
	BRIDGE,
};
class Planet
{
public:
	Planet();
	~Planet();

	void Init(const DirectX::XMFLOAT3 &pos, float size, const DirectX::XMFLOAT4 &color,int stageID, bool Spawn = true,const PlanetType &type = BRIDGE);
	void Update();
	void NormalUpdate();
	void GrabUpdate();
	void SpawnAnimationUpdate();
	void SpawnAnimationStart();
	void Draw();
	void Finalize();
	const Vector3 &GetPos(){return pos;}
	void SetPos(const DirectX::XMFLOAT3 &pos){this->pos = Vector3(pos.x, pos.y, pos.z);}
	void SetScale(float scale);
	Object3D *GetObject3d();
	float GetScale();
	float GetStartScale();
	//この世に存在する状態かどうかのフラグ
	bool GetIsSpawn();

	//出現アニメーションが終了しているかを教える
	bool GetIsSpawnAnimationEnd();

	void ColorChange();
	void ColorOff();
	bool GetIsColorChange();

	std::unique_ptr<PlanetObject> object;
	std::unique_ptr <Model> model;
public:
	static void SetLight(LightGroup *lights);
	static void SetCamera(Camera *cam);
	static void SetShadowCamera(Camera *shadowCam);
	static void SetShadowTexture(int texHandle);
	static void SetPlayerPos(Vector3 playerPos);

	void GrabOn();
	void ReleaseGrab();
	void OnPlayer();
	void ReleaseBase();
	void SetGrabRotateAngle(const DirectX::XMVECTOR &AxisY, const DirectX::XMVECTOR &AxisX);
	void SetNextOrder(bool isNext);
	bool GetIsOnPlayer();
	int GetID();
	bool GetIsNext();
	void SetIsBloom(bool isBloom);
	bool GetIsBloom();
	PlanetType GetType();
	void Reset();
private://静的メンバ変数
	static Camera *cam;
	static Camera *shadowCam;
	static LightGroup *lights;
	static int shadowTextureNum;
	static Vector3 sPlayerPos;
private://メンバ変数

	Vector3 pos;

	bool isGrab_ = false;
	bool isOnPlayer_ = false;
	bool isSpawn_ = false;
	
	bool isSpawnAnimation_ = false;
	DirectX::XMVECTOR GrabRotateAxisY = {};
	DirectX::XMVECTOR GrabRotateAxisX = {};
	float scale = 3.0f;

	Vector3 startPos_;
	float startScale_;
	bool startIsSpawn_;

	PlanetType type_ = NONE;

	//どのステージ番号で出現するか
	int ID_ = -1;
	DirectX::XMFLOAT4 startColor_;
	Easing SpawnAnimationEase_;

	float clearRate = 0.0f;
	float colorRate = 0.0f;

	bool isColorChange_ = false;

	bool isNext_ = false;
	bool isNextLightup_ = true;
	Vector3 ColorOnAngle;

	bool isBloom_ = false;
};

