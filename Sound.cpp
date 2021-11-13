#include "Sound.h"
#include "d3dx12.h"
#include <cassert>
#include "SafeDelete.h"

Microsoft::WRL::ComPtr<IXAudio2> Sound::xAudio2;
IXAudio2MasteringVoice *Sound::masterVoice;

bool Sound::StaticInitialize()
{
	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}
	//�}�X�^�[�{�C�X�𐶐�
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}
}

void Sound::xAudioDelete()
{
	xAudio2.Reset();
}

Sound::SoundData Sound::SoundLoadWave(const char *filename)
{
	//HRESULT result;
#pragma region fileopen
	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	//wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(filename, std::ios_base::binary);
	//�t�@�C���I�[�v�����s�����o����
	
	(file.is_open());
#pragma endregion
#pragma region wavRead
	//RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char *)&riff, sizeof(riff));
	//�t�@�C����Riff���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}
	//���C�y��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	//format�`�����N�̓ǂݍ���
	FormatChunk format = {};
	//�`�����N�w�b�_�̊m�F
	file.read((char *)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}

	//�`�����N�{�̂̓ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char *)&format.fmt, format.chunk.size);

	//Data�`�����N�̓ǂݍ���
	ChunkHeader data;
	file.read((char *)&data, sizeof(data));
	//JUNK�`�����N�����o�����ꍇ
	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		//�ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);
		//�ēǂݍ���
		file.read((char *)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0)
	{
		assert(0);
	}

	//Data�`�����N�̃f�[�^��(�g�`�f�[�^)�̓ǂݍ���
	char *pBuffer = new char[data.size];
	file.read(pBuffer, data.size);


#pragma endregion
#pragma region fileClose
	//WAVE�t�@�C�������
	file.close();
#pragma endregion
#pragma region makeSound
	//return����ׂ̂��񂹂��f�[�^
	SoundData soundData = {};

	this->data.wfex = format.fmt;
	this->data.pBuffer = reinterpret_cast<BYTE *>(pBuffer);
	this->data.bufferSize = data.size;
#pragma endregion

	return soundData;
}

void Sound::SoundUnload()
{
	//�o�b�t�@�̃����������
	delete[] data.pBuffer;

	data.pBuffer = 0;
	data.bufferSize = 0;
	data.wfex = {};
}

void Sound::SoundPlayWave()
{
	{
		HRESULT result;

		//�g�`�t�H�[�}�b�g�����Ƃ�SourceVol�ł̐���
		IXAudio2SourceVoice *pSourceVoice = nullptr;
		result = xAudio2->CreateSourceVoice(&pSourceVoice, &data.wfex);
		assert(SUCCEEDED(result));

		//�Đ�����g�`�f�[�^�̐ݒ�
		XAUDIO2_BUFFER buf{};
		buf.pAudioData = data.pBuffer;
		buf.AudioBytes = data.bufferSize;
		buf.Flags = XAUDIO2_END_OF_STREAM;

		//�g�`�f�[�^�̍Đ�
		result = pSourceVoice->SubmitSourceBuffer(&buf);
		result = pSourceVoice->Start();
	}

}

