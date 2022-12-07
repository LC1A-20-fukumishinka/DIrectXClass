#include "ModelManager.h"

ModelManager::ModelManager()
{

}

ModelManager::~ModelManager()
{
}

void ModelManager::LoadModel(std::string fileName, bool isLoadTexture, bool isSmoothStep)
{
	if(models_.count(fileName) > 0) return;
	//読み込むモデルポインタ
	Model *loadObj;
	loadObj = new Model();
	loadObj->CreateModel(fileName, isLoadTexture, isSmoothStep);

	//名前と一緒にユニークポインタとして格納
	models_.insert(std::make_pair(fileName, loadObj));
}

Model *ModelManager::GetModel(std::string fileName)
{
	//検索して
	auto it = models_.find(fileName);
	//渡す
	return it->second.get();
}
