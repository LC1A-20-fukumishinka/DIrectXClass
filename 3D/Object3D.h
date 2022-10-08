#pragma once
#include "MyDirectX.h"
#include <DirectXMath.h>
#include "Object3DCommon.h"
#include "../Camera.h"
#include "IGraphicsPipeline.h"
#include "../Light.h"
#include "../Collision/CollisionInfo.h"
#include "../LightGroup.h"
class Model;

class BaseCollider;
class Object3D
{
protected:
	struct ConstBufferData
	{
		DirectX::XMFLOAT4 color = {};	//色(RGBA)
		DirectX::XMMATRIX viewproj = {};
		DirectX::XMMATRIX world = {};
		DirectX::XMFLOAT3 cameraPos = {};
	};
public:
	Object3D() = default;

	virtual ~Object3D();
	virtual void Init();

	const DirectX::XMMATRIX GetMatWorld();

	virtual void Update();


	/// <summary>
	/// モデルの描画コマンド
	/// </summary>
	/// <param name="isSetTexture">別のテクスチャを使用する</param>
	/// <param name="textureNumber">使用するテクスチャのハンドル</param>
	virtual void modelDraw(PipeClass::PipelineSet *pipelineSet, bool isSetTexture = false, int textureNumber = -1);
	void SetDrawBuffers(PipeClass::PipelineSet *pipelineSet, bool isSetTexture = false, int textureNumber = -1);
	void DrawCommand();
	virtual void OnCollision(const CollisionInfo &info) {};

public://セッタ

	virtual void SetParent(Object3D *parent);
	virtual void SetCamera(Camera *camera);
	virtual void SetLight(Light *light);
	virtual void SetLightGroup(LightGroup *lightGroup);
	virtual void SetCollider(BaseCollider *collider);
	virtual void SetModel(Model *model);
	virtual void SetPosition(const DirectX::XMFLOAT3 &pos);
	virtual void SetColor(const DirectX::XMFLOAT4 &color);
	virtual void SetScale(const DirectX::XMFLOAT3 &scale);
	/// <summary>
	/// 回転を各軸回転で設定する
	/// </summary>
	/// <param name="rot">Z->X->Y軸順での各軸回転</param>
	virtual void SetRotation(const DirectX::XMFLOAT3 &rot);
	/// <summary>
	/// 回転をquaternionで設定する
	/// </summary>
	/// <param name="quaternion">回転quaternion</param>
	virtual void SetRotation(const DirectX::XMVECTOR &quaternion);
	virtual void AddRotation(const DirectX::XMFLOAT3 &rot);
	virtual void AddRotation(const DirectX::XMVECTOR &rot);
	/// <summary>
	/// 向きたい方向をベクトルで指定する
	/// </summary>
	/// <param name="rot">向く方向</param>
	virtual void SetRotationVector(const DirectX::XMVECTOR &front, const DirectX::XMVECTOR &up = DirectX::XMVECTOR{ 0.0f, 1.0f, 0.0f , 0.0f });
public:
	const DirectX::XMFLOAT4 &GetColor();
	const DirectX::XMFLOAT3 &GetPosition();
	const DirectX::XMFLOAT3 GetWorldPos();
	const DirectX::XMFLOAT3 &GetScale();
	const DirectX::XMFLOAT3 &GetRotation();
	const DirectX::XMVECTOR &GetQuaternion();
	const DirectX::XMVECTOR &GetRotQuaternion();
	const DirectX::XMVECTOR GerWorldQuaternion();
	const DirectX::XMVECTOR &GetUpVec();
	const DirectX::XMVECTOR &GetRightVec();
	const DirectX::XMVECTOR &GetFrontVec();
	const Model *GetModel();
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
	//任意軸回転
	DirectX::XMVECTOR quaternion = {};
	//座標
	DirectX::XMFLOAT3 position = { 0.0f,0.0f ,0.0f };
	DirectX::XMVECTOR front = { 0.0f,0.0f ,1.0f , 0.0f };
	DirectX::XMVECTOR up = { 0.0f,1.0f ,0.0f , 0.0f };
	DirectX::XMVECTOR right = { 1.0f, 0.0f, 0.0f, 0.0f };

	//描画フラグ
	bool isInvisible = false;

	//ワールド行列
	DirectX::XMMATRIX matWorld;

	void UpdateVector();
public://あったほうがいいかもね関数
	/// <summary>
	/// 親子関係を分離する
	/// </summary>
	void Separate();

	void ConnectObject(Object3D *parent);
protected:
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;

	//テクスチャ番号
	UINT texNumber;

	Object3D *parent = nullptr;
	//非表示
	Camera *camera = nullptr;

	Light *light = nullptr;

	LightGroup *lightGroup = nullptr;
	bool isMakeConstBuffer = false;

	Model *model = nullptr;

	ConstBufferData buffData;
};

//深度値リセット
void DepthReset();
