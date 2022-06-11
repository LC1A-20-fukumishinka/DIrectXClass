#pragma once
#include "Object3D.h"
#include "Vector3.h"
#include "Model.h"
#include <memory>
class Planet;
class GravityPlayer
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;
public://基本関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="model"></param>
	void Init(Model *model);
	void Update();
	void Finalize();
	void Draw();

public://ゲッタセッタ
	void SetPos(const DirectX::XMFLOAT3 &pos);

	void SetRotation(const DirectX::XMFLOAT3 &rot);
	void SetModel(Model *model);
	void SetCamera(Camera *cam);
	void SetLight(LightGroup *lights);
	const XMFLOAT3 GetPos();

public://惑星周りの処理
	/// <summary>
	/// 掴む惑星のセットを行う
	/// </summary>
	/// <param name="planet">掴む惑星のポインタ</param>
	void SetGrabPlanet(std::shared_ptr<Planet> planet);

	/// <summary>
	/// 掴んでいる惑星を離す
	/// </summary>
	void ReleasePlanet();

	//掴んでいる際の更新処理ひょっとしたら要らないかも
	void GrabUpdate();
private:
	Vector3 pos;
	Vector3 rotation;
	Vector3 scale;
	Object3D drawObject;
	std::weak_ptr<Planet> grabPlanet;
};

