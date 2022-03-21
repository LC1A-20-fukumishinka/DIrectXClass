#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

#include <fbxsdk.h>
struct Node
{
	//名前
	std::string name;
	//ローカルスケール
	DirectX::XMVECTOR scaling = { 1, 1, 1, 0 };
	//ローカル回転角
	DirectX::XMVECTOR rotation = { 0, 0, 0, 0 };
	//ローカル移動
	DirectX::XMVECTOR translation = { 0, 0, 0, 1 };
	//ローカル変形行列
	DirectX::XMMATRIX transform;
	//グローバル変形行列
	DirectX::XMMATRIX globalTransform;
	//親ノード
	Node *parent = nullptr;
};
class FbxModel
{
public:
	//フレンドクラス
	friend class FbxLoader;
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	using string = std::string;
	template <class T> using vector = std::vector<T>;
public://定数
	static const int MAX_BONE_INDICES = 4;
public:
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		UINT boneIndex[MAX_BONE_INDICES];	//ボーン 番号
		float boneWeight[MAX_BONE_INDICES];	//ボーン 重み
	};

	struct Material
	{
		DirectX::XMFLOAT3 ambient = { 1, 1, 1 };
		DirectX::XMFLOAT3 diffuse = { 1, 1, 1 };
		//テクスチャー用のハンドル
		int textureHandle;
	};

	struct Bone
	{
		//名前
		std::string name;
		//初期姿勢の逆行列
		DirectX::XMMATRIX invInitialPose;
		//クラスター(FBX側のボーン情報)
		FbxCluster *fbxCluster;
		//コンストラクタ
		Bone(const std::string &name)
		{
			this->name = name;
		}
	};
public://アクセッサ
	FbxScene *GetFbxScene(){return fbxScene;}

	std::vector<Bone> &GetBones() { return bones; }
public:
	~FbxModel();

	void CreateBuffers();
	void Draw();

	const XMMATRIX &GetModeTransform() { return meshNode->globalTransform; }
private:
	//モデル名
	std::string name;
	//ノード配列
	std::vector<Node> nodes;
	//メッシュを持つノード
	Node *meshNode = nullptr;
	//頂点データ配列
	std::vector<VertexPosNormalUvSkin> vertices;
	//頂点インデックス
	std::vector<unsigned short> indices;
	//マテリアル
	Material material;

	ComPtr<ID3D12Resource> vertBuff;
	ComPtr<ID3D12Resource> indexBuff;

	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	D3D12_INDEX_BUFFER_VIEW ibView = {};

	//ボーン配列
	std::vector<Bone> bones;
	//FBXシーン
	FbxScene *fbxScene = nullptr;
};