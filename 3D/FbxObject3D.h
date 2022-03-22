#pragma once
#include "FbxModel.h"
#include "../Camera.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class FbxObject3D
{
protected:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public://定数
	//ボーンの最大数
	static const int MAX_BONES = 32;
public:
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};
	//定数バッファ用データ構造体(スキニング)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};
public:
	static void CreateGraphicsPipeline();
	static void SetCamera(Camera *camera) { FbxObject3D::camera = camera; }
	static void SetDevice();
public:
	void Init();
	void Update();
	void Draw();
	void SetModel(FbxModel *model) { this->model = model; }

	/// <summary>
	/// アニメーション開始
	/// </summary>
	void PlayAnimation();

public:
	void SetPosition(const XMFLOAT3 &position) { this->position = position; }
	void SetRotation(const XMFLOAT3 &rotation) { this->rotation = rotation; }
	void SetScale(const XMFLOAT3 &scale) { this->scale = scale; }
	const XMFLOAT3 &GetPosition() { return position; }
	const XMFLOAT3 &GetRotation() { return rotation; }
	const XMFLOAT3 &GetScale() { return scale; }
	const XMMATRIX &GetMatWorld() { return matWorld; }

private:
	static ID3D12Device *device;
	static Camera *camera;

	ComPtr<ID3D12Resource> constBufferTransform;
	static ComPtr<ID3D12RootSignature> rootsignature;
	static ComPtr<ID3D12PipelineState> pipelinestate;
	//定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBuffSkin;
private:
	XMFLOAT3 scale = { 1, 1, 1 };
	XMFLOAT3 rotation = { 0, 0, 0 };
	XMFLOAT3 position = { 0, 0, 0 };
	XMMATRIX matWorld;

	FbxModel *model = nullptr;

	//1フレームの時間
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間（アニメーション）
	FbxTime currentTime;
	//アニメーション再生中
	bool isPlay = false;
};