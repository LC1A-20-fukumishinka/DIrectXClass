#pragma once
#include "MyDirectX.h"
#include <DirectXMath.h>
#include "Object3DCommon.h"
#include "../Camera.h"
#include "IGraphicsPipeline.h"
#include "../Light.h"
#include "../Collision/CollisionInfo.h"
struct ModelObject;

class BaseCollider;
class Object3D
{
public:

	struct ConstBufferData
	{
		DirectX::XMFLOAT4 color;	//色(RGBA)
		//DirectX::XMMATRIX mat;	//3D変換行列
		DirectX::XMMATRIX viewproj;
		DirectX::XMMATRIX world;
		DirectX::XMFLOAT3 cameraPos;
	};
	enum ObjectName
	{
		Corn,
		Box,
		Plane
	};
	Object3D() = default;

	virtual ~Object3D();
	virtual void Init(Camera *camera, Light *light, Object3D *parent = nullptr);

	const DirectX::XMMATRIX GetMatWorld();

	virtual void Update();


	/// <summary>
	/// モデルの描画コマンド
	/// </summary>
	/// <param name="isSetTexture">別のテクスチャを使用する</param>
	/// <param name="textureNumber">使用するテクスチャのハンドル</param>
	virtual void modelDraw(PipeClass::PipelineSet pipelineSet, bool isSetTexture = false, int textureNumber = -1);

	virtual void OnCollision(const CollisionInfo &info) {};

public://セッタ

	void SetParent(Object3D *parent);

	void SetCamera(Camera *camera);

	void SetLight(Light *light);

	void SetCollider(BaseCollider *collider);

	void SetModel(ModelObject *model);

	void SetPosition(DirectX::XMFLOAT3 pos);
	void SetColor(DirectX::XMFLOAT4 color);
	void SetScale(DirectX::XMFLOAT3 scale);
	void SetRotation(DirectX::XMFLOAT3 rot);
public:
	const DirectX::XMFLOAT4 &GetColor();
	const DirectX::XMFLOAT3 &GetPosition();
	const DirectX::XMFLOAT3 &GetScale();
	const DirectX::XMFLOAT3 &GetRotation();
protected:
	//クラス名(デバッグ用)
	const char *name = nullptr;

	//コライダー
	BaseCollider *collider = nullptr;

	//色(RGBA)
	DirectX::XMFLOAT4 color = { 1.0f,1.0f ,1.0f ,1.0f };
	//大きさ
	DirectX::XMFLOAT3 scale = { 1.0f ,1.0f ,1.0f };
	//回転
	DirectX::XMFLOAT3 rotation = { 0.0f,0.0f ,0.0f };
	//座標
	DirectX::XMFLOAT3 position = { 0.0f,0.0f ,0.0f };
	//描画フラグ
	bool isInvisible = false;

	//ワールド行列
	DirectX::XMMATRIX matWorld;

private:
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;

	//テクスチャ番号
	UINT texNumber;

	Object3D *parent = nullptr;
	//非表示
	Camera *camera = nullptr;

	Light *light = nullptr;

	bool isMakeConstBuffer = false;

	ModelObject *model = nullptr;
};

//深度値リセット
void DepthReset();
