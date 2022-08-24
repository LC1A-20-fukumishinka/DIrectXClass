#pragma once
#include "Object3D.h"
#include <memory>
#include "Vector3.h"

class Model;
class Planet;

/// <summary>
/// 球面上に配置されるオブジェクトクラス
/// </summary>
class ObjectOnSphere
{
public://基礎関数
	ObjectOnSphere();
	~ObjectOnSphere();

	virtual void Init(std::weak_ptr<Planet> basePlanet, Vector3 angle, float scale);
	virtual void Update();
	virtual void Draw();
	virtual void Finalize();

protected://メンバ関数

	//Modelを割り当てる用
	void SetModel(Model *model);


	Vector3 GetWorldPos();
	float GetWorldScale();
	DirectX::XMVECTOR GetRotationQ();

	bool GetIsPlanetSpawn();
	///// <summary>
	///// 引数で入れた動かす動かす
	///// </summary>
	///// <param name="MoveVec">動かすベクトル</param>
	//void Move(Vector3 MoveVec);
protected://staic変数
	//ゲームのメインカメラ
	static Camera *sMainCamera;
	//ゲームのライト
	static LightGroup *sLights;

public://static関数

	/// <summary>
	/// ゲーム内のメインカメラのセッタ
	/// </summary>
	/// <param name="camera">割る当てるカメラのポインタ(最終的にはスマートポインタ化しろ)</param>
	static void SetCamera(Camera *camera);

	/// <summary>
	/// 諸々光源
	/// </summary>
	/// <param name="lights">割り当てる光源</param>
	static void SetLights(LightGroup *lights);
private://メンバ関数
	void RotationUpdate();
private://メンバ変数

	Object3D shadowObject_;
	Object3D object_;

	std::weak_ptr<Planet> basePlanet_;

	Vector3 upVec_ = { 0, 1, 0 };
	Vector3 frontVec_ = { 0, 0, 1 };
	float worldScale_ = 1.0f;
};



