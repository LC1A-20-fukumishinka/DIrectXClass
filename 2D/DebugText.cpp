#include "DebugText.h"

DebugText::DebugText()
{
	spriteIndex = 0;
}

void DebugText::Initialize(ID3D12Device *dev, int window_width, int window_height,const SpriteCommon &spriteCommon)
{
	//�S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(sprites); i++)
	{
		//�X�v���C�g�𐶐�����
		sprites[i].Init(window_width, window_height, debugTextTexNumber, spriteCommon, { 0,0 });
	}
}
void DebugText::Print(const SpriteCommon &spriteCommon, const std::string &text, float x, float y, float scale)
{
	//�S�Ă̕����ɂ���
	for (int i = 0; i < text.size(); i++)
	{
		//�ő啶��������
		if (spriteIndex >= maxCharCount)
		{
			break;
		}

		//�ꕶ�����o��(��ASCII�R�[�h�ł��������藧���Ȃ�)
		const unsigned char &character = text[i];

		//ASCII�R�[�h�̓�i����΂����ԍ����v�Z
		int fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		//���W�v�Z
		sprites[spriteIndex].position = { x + fontWidth * scale * i, y, 0 };
		sprites[spriteIndex].texLeftTop = { (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight };
		sprites[spriteIndex].texSize = { fontWidth, fontHeight };
		sprites[spriteIndex].size = { fontWidth * scale, fontHeight * scale };
		//���_�o�b�t�@�]��
		sprites[spriteIndex].SpriteTransferVertexBuffer(spriteCommon);
		//�X�V
		sprites[spriteIndex].SpriteUpdate(spriteCommon);
		//��������i�߂�
		spriteIndex++;
	}

}
void DebugText::DrawAll(ID3D12GraphicsCommandList *cmdList, const SpriteCommon &spriteCommon, ID3D12Device *dev)
{
	//�S�Ă̕����̃X�v���C�g�ɂ���
	for (int i = 0; i < spriteIndex; i++)
	{
		//�X�v���C�g�`��
		sprites[i].SpriteDraw(spriteCommon);
	}

	spriteIndex = 0;
}

void DebugText::LoadDebugTextTexture(SpriteCommon &spriteCommon)
{
	//�f�o�b�O�e�N�X�g�p�̃e�N�X�`���ǂݍ���
	spriteCommon.SpriteLoadTexture(DebugText::debugTextTexNumber, L"Resources/debugfont.png");
}
