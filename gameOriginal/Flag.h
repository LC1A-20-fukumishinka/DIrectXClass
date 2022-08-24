#pragma once
#include "ObjectOnSphere.h"

class Flag : public ObjectOnSphere
{
public:
	Flag();
	~Flag();

	void Init(std::weak_ptr<Planet> basePlanet, Vector3 angle, float scale) override;
	void Update() override;
	void Finalize() override;
	void Draw()override;

	void DrawShadow();

private://staic変数
	static std::unique_ptr<Model> sFlagModel;
	static Camera *sShadowCamera;
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
	bool CollisionPlayer(float radius, const Vector3 pos);

	bool GetIsDraw();

	void Reset();
private://メンバ変数

	Object3D shadowObject_;

	float worldScale_ = 1.0f;

	bool isDraw_ = true;
};

