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
bool LoadGateFile(int stage , std::vector<std::shared_ptr<Gate>> &gates);

/// <summary>
/// 惑星データの保存
/// </summary>
/// <param name="planets">惑星データ</param>
/// <param name="Gates">ゲートデータ</param>
/// <param name="isStartDraw">初期描画</param>
void SaveStageFile(const std::vector<std::weak_ptr<Planet>>& planets, const  std::vector<std::weak_ptr<Gate>>& Gates, bool isStartDraw);


void SaveAllStageFile(const std::list<std::shared_ptr<Planet>>& planets, const  std::vector<std::shared_ptr<Gate>>& Gates);