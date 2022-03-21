#pragma once

#include "fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include "FbxModel.h"
class FbxLoader
{
private:
	using string = std::string;
public:
	static const string baseDirectory;
	//テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader *GetInstance();

	/// <summary>
	/// FBXの行列をXMMatrixに変換
	/// </summary>
	/// <param name="dst">書き込み先</param>
	/// <param name="src">元となるFBX行列</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX *dst, const FbxAMatrix &src);
private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader &obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader &obj) = delete;
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize();

	//void LoadModelFromFile(const string &modelName);

	FbxModel *LoadModelFromFile(const string &modelName);


	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name="model">読み込み先も出るオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	/// <param name="parent">親ノード</param>
	void ParseNodeRecursive(FbxModel *model, FbxNode *fbxNode, Node *parent = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="model">読み込み先も出るオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseMesh(FbxModel *model, FbxNode *fbxNode);

private:
	//頂点座標読み取り
	void ParseMeshVertices(FbxModel *model, FbxMesh *fbxMesh);
	//面情報読み取り
	void ParseMeshFaces(FbxModel *model, FbxMesh *fbxMesh);
	//マテリアル読み取り
	void ParseMaterial(FbxModel *model, FbxNode *fbxNode);
	//テクスチャ読み込み
	void LoadTexture(FbxModel *model, const std::string &fullpath);
	//ディレクトリを含んだファイルパスからファイル名を抽出
	std::string ExtractFileName(const std::string &path);
	//スキニング情報の読み取り
	void ParseSkin(FbxModel *model, FbxMesh *fbxMesh);
private:
	FbxManager *fbxManager = nullptr;

	FbxImporter *fbxImporter = nullptr;
};