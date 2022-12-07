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
	//�ǂݍ��ރ��f���|�C���^
	Model *loadObj;
	loadObj = new Model();
	loadObj->CreateModel(fileName, isLoadTexture, isSmoothStep);

	//���O�ƈꏏ�Ƀ��j�[�N�|�C���^�Ƃ��Ċi�[
	models_.insert(std::make_pair(fileName, loadObj));
}

Model *ModelManager::GetModel(std::string fileName)
{
	//��������
	auto it = models_.find(fileName);
	//�n��
	return it->second.get();
}
