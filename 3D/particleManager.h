#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>
#include "../Camera.h"
#include "Vector3.h"
#include "IGraphicsPipeline.h"
#include <memory>
/// <summary>
/// 3Dオブジェクト
/// </summary>
class ParticleManager
{
private: // エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public: // サブクラス
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT4 color;
		float scale; //スケール

	};
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX mat;	// ３Ｄ変換行列
		XMMATRIX matBillboard;	// ３Ｄ変換行列
	};

	struct Particle
	{
		//DirectX::を省略
		using XMFLOAT3 = DirectX::XMFLOAT3;

		//座標
		Vector3 position = {};
		//速度
		Vector3 velocity = {};
		//加速度
		Vector3 accel = {};
		//現在のフレーム
		int frame = 0;
		//終了フレーム
		int dead_frame = 0;
		//スケール
		float scale = 1.0f;
		//初期値
		float s_scale = 1.0f;
		//最終値
		float e_scale = 0.0f;

		XMVECTOR s_color = { 1, 1, 1, 0 };

		XMVECTOR e_color = { 1, 1, 1, 1 };
		XMFLOAT4 draw_color = { 1, 1, 1, 1 };
	};
private: // 定数
	static const int vertexCount = 256;		// 頂点数
public: // 静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <returns>成否</returns>
	static bool StaticInitialize();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static ParticleManager *Create();


private: // 静的メンバ変数
	// デバイス
	static ID3D12Device *device;

private:// 静的メンバ関数

	///// <summary>
	///// テクスチャ読み込み
	///// </summary>
	///// <returns>成否</returns>
	//static bool LoadTexture();


public: // メンバ関数
	ParticleManager();

	void Init();

	/// <summary>
	/// 使用するカメラの設定
	/// </summary>
	void SetCamera(Camera *camera);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="textureNumber">描画するテクスチャのインデックス</param>
	void Draw();

	/// <summary>
	/// パーティクル生成
	/// </summary>
	/// <param name="life">描画の時間</param>
	/// <param name="position">場所</param>
	/// <param name="velocity">移動ベクトル</param>
	/// <param name="accel">加速ベクトル</param>
	/// <param name="start_scale">初期サイズ</param>
	/// <param name="end_scale">終了時サイズ</param>
	void Add(int life, const XMFLOAT3 &position, const XMFLOAT3 &velocity, const XMFLOAT3 &accel,
		float start_scale, float end_scale, const XMFLOAT4 &start_color = { 1, 1, 1, 1 }, const XMFLOAT4 &end_color = { 1, 1, 1,1 });

	void SetTexture(int textureHandle);

	void SendBuffers(std::forward_list<Particle> &particles);

protected:
	std::forward_list<Particle> particles_;
private: // メンバ変数
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ

	// ローカルスケール
	Vector3 scale = { 1,1,1 };


	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;

	VertexPos vertices[vertexCount];

	Camera *camera;

	bool isBillboard_ = true;

	int textureHandle_ = -1;
	//// ルートシグネチャ
	//ComPtr<ID3D12RootSignature> rootsignature;
	//// パイプラインステートオブジェクト
	//ComPtr<ID3D12PipelineState> pipelinestate;

	std::unique_ptr<PipeClass::GSPipelineSet> pipelineSet;
private:

	/// <summary>
/// モデル作成
/// </summary>
	void CreateModel();

	/// <summary>
/// グラフィックパイプライン生成
/// </summary>
/// <returns>成否</returns>
	bool InitializeGraphicsPipeline();
};

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3 &lhs, const DirectX::XMFLOAT3 &rhs);
