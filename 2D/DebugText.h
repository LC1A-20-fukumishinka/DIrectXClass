#pragma once
#include "Sprite.h"
//�f�o�b�O������N���X�̒�`
class DebugText
{
public:
	//�����ɒ萔�̐錾���L�q
	static const int maxCharCount = 256;
	static const int fontWidth = 9;
	static const int fontHeight = 18;
	static const int fontLineCount = 14;
	static const int debugTextTexNumber = 2;
	//�����Ƀ����o�֐��̐錾���L�q
	DebugText();

	void Initialize(ID3D12Device *dev);
	void Print( const std::string &text, float x, float y, float scale = 1.0f);
	void DrawAll();
private:
	//�����Ƀ����o�ϐ��̒�`���L�q

	//�X�v���C�g�̔z��
	Sprite sprites[maxCharCount];
	//�X�v���C�g�z��̓Y�����ԍ�
	int spriteIndex;

	//�e�N�X�`���[�ԍ�
	int debugTex;

};