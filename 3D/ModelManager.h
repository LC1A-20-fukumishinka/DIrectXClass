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
	/// ���f���}�l�[�W���[�Ƀ��f����ǂݍ��ݒǉ�����
	/// </summary>
	/// <param name="fileName">�ǂݍ��ރ��f���̖��O</param>
	/// <param name="isLoadTexture">�e�N�X�`����ǂݍ��ނ�</param>
	/// <param name="isSmoothStep">���f���@���̕��ω����s����</param>
	void LoadModel(std::string fileName, bool isLoadTexture = true, bool isSmoothStep = false);

	/// <summary>
	/// �w�肵�����O�̃��f���̃|�C���^���󂯎��
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <returns></returns>
	Model *GetModel(std::string fileName);
private:

std::map<std::string, std::unique_ptr<Model>> models_;
};

