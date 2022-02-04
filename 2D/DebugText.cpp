#include "DebugText.h"
#include "TextureMgr.h"
DebugText::DebugText()
{
	spriteIndex = 0;
	debugTex = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/debugfont.png");

	//�S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(sprites); i++)
	{
		//�X�v���C�g�𐶐�����
		sprites[i].Init(debugTex, { 0,0 });
	}
}

DebugText *DebugText::Create()
{
	DebugText *debugText = new DebugText();
	if (debugText == nullptr) {
		return nullptr;
	}
	return debugText;
}

void DebugText::Print( const std::string &text, float x, float y, float scale)
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
		sprites[spriteIndex].position = { x + fontWidth * scale * i, y};
		sprites[spriteIndex].texLeftTop = { (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight };
		sprites[spriteIndex].texSize = { fontWidth, fontHeight };
		sprites[spriteIndex].size = { fontWidth * scale, fontHeight * scale };
		//���_�o�b�t�@�]��
		sprites[spriteIndex].SpriteTransferVertexBuffer();
		//�X�V
		sprites[spriteIndex].Update();
		//��������i�߂�
		spriteIndex++;
	}

}
void DebugText::DrawAll()
{
	//�S�Ă̕����̃X�v���C�g�ɂ���
	for (int i = 0; i < spriteIndex; i++)
	{
		//�X�v���C�g�`��
		sprites[i].Draw();
	}

	spriteIndex = 0;
}

