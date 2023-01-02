#pragma once
#include "Planet.h"
#include "Gate.h"
#include <memory>

/// <summary>
/// テキストから惑星のデータを読み込む
/// </summary>
/// <param name="stage">ステージ番号</param>
/// <param name="planets">惑星のデータを入れる配列</param>
/// <returns>読み込みに成功したか</returns>
bool LoadStageFile(int stage , std::vector<std::shared_ptr<Planet>> &planets);

/// <summary>
/// テキストからゲートリングのデータを読み込む
/// </summary>
/// <param name="stage">ステージ番号</param>
/// <param name="gates">ゲートリングのデータを入れる配列</param>
/// <returns>読み込みに成功したか</returns>
bool LoadGateFile(int stage , std::vector<std::unique_ptr<Gate>> &gates);

void SaveStageFile(int stage, std::vector<std::shared_ptr<Planet>>& planets);