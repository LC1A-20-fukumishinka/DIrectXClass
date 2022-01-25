#pragma once
#include "Sprite.h"
//デバッグ文字列クラスの定義
class DebugText
{
public:
	//ここに定数の宣言を記述
	static const int maxCharCount = 256;
	static const int fontWidth = 9;
	static const int fontHeight = 18;
	static const int fontLineCount = 14;
	static const int debugTextTexNumber = 2;
	//ここにメンバ関数の宣言を記述
private://メンバ関数
	DebugText();
public://メンバ関数
	static DebugText *Create();
	void Print(const std::string &text, float x, float y, float scale = 1.0f);
	void DrawAll();
private:
	//ここにメンバ変数の定義を記述

	//スプライトの配列
	Sprite sprites[maxCharCount];
	//スプライト配列の添え字番号
	int spriteIndex;

	//テクスチャー番号
	int debugTex;

};