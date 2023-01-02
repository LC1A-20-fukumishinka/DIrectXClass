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
bool LoadGateFile(int stage , std::vector<std::unique_ptr<Gate>> &gates);

void SaveStageFile(int stage, std::vector<std::shared_ptr<Planet>>& planets);