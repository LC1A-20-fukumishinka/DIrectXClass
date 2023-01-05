#pragma once
#include "Planet.h"
#include "Gate.h"
#include <memory>

/// <summary>
/// �e�L�X�g����f���̃f�[�^��ǂݍ���
/// </summary>
/// <param name="stage">�X�e�[�W�ԍ�</param>
/// <param name="planets">�f���̃f�[�^������z��</param>
/// <returns>�ǂݍ��݂ɐ���������</returns>
bool LoadStageFile(int stage , std::vector<std::shared_ptr<Planet>> &planets);

/// <summary>
/// �e�L�X�g����Q�[�g�����O�̃f�[�^��ǂݍ���
/// </summary>
/// <param name="stage">�X�e�[�W�ԍ�</param>
/// <param name="gates">�Q�[�g�����O�̃f�[�^������z��</param>
/// <returns>�ǂݍ��݂ɐ���������</returns>
bool LoadGateFile(int stage , std::vector<std::shared_ptr<Gate>> &gates);

/// <summary>
/// �f���f�[�^�̕ۑ�
/// </summary>
/// <param name="planets">�f���f�[�^</param>
/// <param name="Gates">�Q�[�g�f�[�^</param>
/// <param name="isStartDraw">�����`��</param>
void SaveStageFile(const std::vector<std::weak_ptr<Planet>>& planets, const  std::vector<std::weak_ptr<Gate>>& Gates, bool isStartDraw);


void SaveAllStageFile(const std::list<std::shared_ptr<Planet>>& planets, const  std::vector<std::shared_ptr<Gate>>& Gates);