#pragma once
#include "Object3D.h"
#include <memory>
#include "Vector3.h"
#include "IGraphicsPipeline.h"

class Model;
class Planet;

// 現在抱えている問題
//
//・光の姿勢がバグる




/// <summary>
/// 旗
/// </summary>
class Flag 
{
public:
	Flag();
	~Flag();

	void Init(std::weak_ptr<Planet> basePlanet,const Vector3 &angle, float scale);
	void Update() ;
	void Finalize() ;
	void Draw();

	void DrawShadow();

private://staic変数
	static std::unique_ptr<Model> sFlagModel;
	static std::unique_ptr<Model> sLightPillarModel;
	static Camera *sShadowCamera;
	//ゲームのメインカメラ
	static Camera *sMainCamera;
	//ゲームのライト
	static LightGroup *sLights;

	static int testTex;
	static std::unique_ptr<PipeClass::PipelineSet> sLightPipeline;
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

void LightPillarUpdate();
public://static関数


	/// <summary>
	/// シャドウカメラのセッタ
	/// </summary>
	/// <param name="camera">割り当てるカメラのポインタ(メインカメラ同様スマートポインタ化しろ)</param>
	static void SetShadowCamera(Camera *camera);

public:

	/// <summary>
	/// Flagを獲得するための関数
	/// </summary>
	/// <param name="radius">半径</param>
	/// <param name="pos">座標</param>
	/// <returns>接触したかどうか</returns>
	bool CollisionPlayer(float radius, const Vector3 &pos);

	bool GetIsDraw();

	void Reset();
private://メンバ変数

	Object3D shadowObject_;
	Object3D lightPillarObject_;
	bool isDraw_ = true;

	Object3D object_;

	std::weak_ptr<Planet> basePlanet_;

	Vector3 upVec_ = { 0, 1, 0 };
	Vector3 frontVec_ = { 0, 0, 1 };
	float worldScale_ = 0.0f;
	Microsoft::WRL::ComPtr<ID3D12Resource> lightConstBuff2;
	float lightRate_ = 1.0f;

	bool lightAnimationFlag_ = true;

	float lightAnimationRate_ = 0.0f;
};

