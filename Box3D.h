#pragma once
#include "MyDirectX.h"
#include <DirectXMath.h>
#include "Object3DCommon.h"
#include "../Camera.h"
#include "IGraphicsPipeline.h"
#include "../Light.h"
#include "../Collision/CollisionInfo.h"
#include "IGraphicsPipeline.h"
#include "Vector3.h"
/// <summary>
/// ライティングなしの矩形オブジェクト
/// </summary>
class BoxObj3D
{
private:
	struct ConstBufferData
	{
		DirectX::XMMATRIX mat;//ワールド行列(回転とスケーリングのみ)
		DirectX::XMMATRIX matViewprj;//ビュープロジェクション行列
		DirectX::XMFLOAT4 color;//色
	};
public:
	BoxObj3D();
	~BoxObj3D();

	void Init(const Vector3 pos, const Vector3 &front , const Vector3 &up);
	void Update();
	void Draw();
	void Finalize();

	const DirectX::XMMATRIX GetMatWorld();

	void SetCamera(Camera *camera);


	void SetRotation(DirectX::XMFLOAT3 rot);

	void SetRotation(DirectX::XMVECTOR quaternion);

	void AddRotation(DirectX::XMFLOAT3 rot);

	void AddRotation(DirectX::XMVECTOR rot);

	/// <summary>
	/// 姿勢を正面と上ベクトルから計算する
	/// </summary>
	/// <param name="front"></param>
	/// <param name="up"></param>
	void SetRotationVector(DirectX::XMVECTOR front, DirectX::XMVECTOR up);

private://スタティック変数
	static std::unique_ptr<PipeClass::PipelineSet> pipelineSet_;
private:
	void MakeVertexBuffer();
	void MakeIndexBuffer();
	void MakeConstBuffer();
private:
	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;

	void UpdateVector();

	//色(RGBA)
	DirectX::XMFLOAT4 color = { 1.0f,1.0f ,1.0f ,1.0f };
	//大きさ
	Vector3 scale = { 10.0f ,10.0f ,10.0f };
	//任意軸回転
	DirectX::XMVECTOR quaternion = {};
	//座標
	Vector3 position = { 0.0f,0.0f ,0.0f };
	DirectX::XMVECTOR front = { 0.0f,0.0f ,1.0f , 0.0f };
	DirectX::XMVECTOR up = { 0.0f,1.0f ,0.0f , 0.0f };
	DirectX::XMVECTOR right = { 1.0f, 0.0f, 0.0f, 0.0f };

	//描画フラグ
	bool isInvisible = false;

	//ワールド行列
	DirectX::XMMATRIX matWorld;

	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;

	//テクスチャ番号
	UINT texNumber;

	Object3D *parent = nullptr;
	//非表示
	Camera *camera = nullptr;

	Light *light = nullptr;

	bool isMakeBuffers = false;


	ConstBufferData buffData;


};

