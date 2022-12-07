#pragma once
#include "Model.h"
#include <map>
#include <memory>
#include <string>
class ModelManager
{

public:
	ModelManager();
	~ModelManager();

	/// <summary>
	/// モデルマネージャーにモデルを読み込み追加する
	/// </summary>
	/// <param name="fileName">読み込むモデルの名前</param>
	/// <param name="isLoadTexture">テクスチャを読み込むか</param>
	/// <param name="isSmoothStep">モデル法線の平均化を行うか</param>
	void LoadModel(std::string fileName, bool isLoadTexture = true, bool isSmoothStep = false);

	/// <summary>
	/// 指定した名前のモデルのポインタを受け取る
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns></returns>
	Model *GetModel(std::string fileName);
private:

std::map<std::string, std::unique_ptr<Model>> models_;
};

