#include "Base/WinAPI.h"
#include "Base/MyDirectX.h"
#include "Input/DirectInput.h"
#include "2D/SpriteCommon.h"
#include "2D/Sprite.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <math.h>
#include <DirectXTex.h>
#include <wrl.h>
#include <xaudio2.h>
#include <fstream>
#include "2D/DebugText.h"
#include "3D/Object3DCommon.h"
#include "3D/Object3D.h"
#include "Camera.h"
#include "EaseClass.h"
#include "Sound.h"
#include "Base/GraphicsPipeline3D.h"
#include "IGraphicsPipeline.h"
#include "TextureMgr.h"
#include "Model.h"
#include "ModelPipeline.h"
using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "xaudio2.lib")


struct Vertex
{
	XMFLOAT3 pos;	//xyz���W
	XMFLOAT3 normal;
	XMFLOAT2 uv;	//uv���W
};

struct ConstBufferData
{
	XMFLOAT4 color;	//�F(RGBA)
	XMMATRIX mat;	//3D�ϊ��s��
};


//const int spriteSRVCount = 512;


const int window_width = 1280;
const int window_height = 720;

//#pragma region Sound
////�`�����N�w�b�_
//struct ChunkHeader
//{
//	char id[4];		//�`�����N����ID
//	int32_t size;	//�`�����N�T�C�Y
//};
////RIFF�w�b�_�`�����N
//struct RiffHeader
//{
//	ChunkHeader chunk;	//"RIFF"
//	char type[4];		//"WAVE"
//};
////FMT�`�����N
//struct FormatChunk
//{
//	ChunkHeader chunk;	//"fmt"
//	WAVEFORMATEX fmt;	//�g�`�t�H�[�}�b�g
//};
////�����f�[�^
//struct SoundData
//{
//	//�g�`�t�H�[�}�b�g
//	WAVEFORMATEX wfex;
//	//�o�b�t�@�̐擪�A�h���X
//	BYTE *pBuffer;
//	//�o�b�t�@�̃T�C�Y
//	unsigned int bufferSize;
//};
//
////�����f�[�^�ǂݍ���
//SoundData SoundLoadWave(const char *filename);
//
////�����f�[�^���
//void SoundUnload(SoundData *soundData);
//
////�����Đ�
//void SoundPlayWave(IXAudio2 *xAudio2, const SoundData &soundData);
//#pragma endregion

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//WindowsAPI����������
#pragma region WindowsAPI

	WinAPI *Win = WinAPI::GetInstance();

	Win->Init(window_width, window_height);

	//SpriteCommon spriteCommon;


#pragma endregion

//HRESULT result;

#pragma region sound(xAudioInstance)
	//�錾
	//ComPtr<IXAudio2> xAudio2;
	//IXAudio2MasteringVoice *masterVoice;
	//result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	////�}�X�^�[�{�C�X�𐶐�
	//result = xAudio2->CreateMasteringVoice(&masterVoice);

	Sound::StaticInitialize();
	Sound alarm;
	alarm.SoundLoadWave("Resources/Alarm01.wav");
#pragma endregion

	//DirectX���������� �����܂�
	MyDirectX *myDirectX = MyDirectX::GetInstance();
	//DirectInput�̏�����������������

	IGraphicsPipeline *Pipe3D = GraphicsPipeline3D::GetInstance();
	IGraphicsPipeline *model3D = ModelPipeline::GetInstance();
#pragma region DirectInput
		Input * input = Input::GetInstance();

	input->Init(Win->w, Win->hwnd);
#pragma endregion


	//�萔�o�b�t�@����
#pragma region ViewMatrix

	Camera cam;
	cam.Init(XMFLOAT3(0, 0, -100), XMFLOAT3(0, 0, 0));
	float angle = 0.0f;	//�J�����̉�]�p

#pragma endregion

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
#pragma region gpipeline
#pragma region brend
		//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	//D3D12_RENDER_TARGET_BLEND_DESC &blenddesc = gpipeline.BlendState.RenderTarget[0];
	//blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//�W���ݒ�
//#pragma region NOBLEND
//	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
//	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
//	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X(�`�悷��}�`�̃s�N�Z��)�̒l��100%�g��
//	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g(�`��Ώۃs�N�Z���@�@�@)�̒l��  0%�g��
//#pragma endregion
//
//#pragma region ADD
//	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//���Z
//	blenddesc.SrcBlend = D3D12_BLEND_ONE;	//�\�[�X�̒l��100%�g��
//	blenddesc.DestBlend = D3D12_BLEND_ONE;	//�f�X�g�̒l��100%�g��
//#pragma endregion
//
//#pragma region SUB
//	blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//���Z
//	blenddesc.SrcBlend = D3D12_BLEND_ONE;				//�\�[�X�̒l��100%�g��
//	blenddesc.DestBlend = D3D12_BLEND_ONE;				//�f�X�g�̒l��100%�g��
//#pragma endregion
//
//#pragma region INVERSION
//	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
//	blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	//1.0f - �f�X�g�J���[�̒l
//	blenddesc.DestBlend = D3D12_BLEND_ZERO;				//�g��Ȃ�
//#pragma endregion
//
//#pragma region ALPHA
//	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
//	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
//	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f - �\�[�X�̃A���t�@�l
//#pragma endregion
#pragma endregion
	int spriteTex = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/texture.png");
	Sprite sprite;
	sprite.Init(spriteTex);


	//�f�o�b�O�e�L�X�g
#pragma region DebugText
	//�f�o�b�O�e�L�X�g�̃C���X�^���X
	DebugText debugText;
	////�f�o�b�O�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	//�f�o�b�O�e�N�X�g�p�̃e�N�X�`���ǂݍ���

	Object3DCommon obC;


	obC.Init();

	int testTexNum = 0;
	int gridTexNum = 1;

	int test = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/texture.png");
	int grid = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/gridWall.png");
	Model triangle;

	//BillBoard bill;

	//bill.Init(obC, cam, testTexNum);
	//bill.obj.scale = { 10,10,10 };

	Object3D floor[100] = {};


	Easing EQ;
	EQ.Init(60);
	//10�̈�(�s)
	for (int i = 0; i < 10; i++)
	{
		floor[i * 10].Init(obC, cam, gridTexNum);
		floor[i * 10].type = Object3D::Plane;
		//�擪��0�Ԗڂ̃I�u�W�F�N�g����ɂȂ�̂Ŕ�΂�
		if (i > 0)
		{
			//���̃I�u�W�F�N�g��e��
			floor[i * 10].SetParent(&floor[(i - 1) * 10]);
			//1�}�X�����ɂ����
			floor[i * 10].position = { 0, 2, 0 };
		}
		//1�̈�(��)
		for (int j = 1; j < 10; j++)
		{
			int num = i * 10 + j;

			floor[num].Init(obC, cam, gridTexNum);
			floor[num].type = Object3D::Plane;
			//����̃I�u�W�F�N�g��e��
			floor[num].SetParent(&floor[num - 1]);
			//1�}�X���E�ɂ����
			floor[num].position = { 2, 0, 0 };
		}
	}

	floor[0].scale = { 10,10,10 };
	floor[0].rotation = { 0 , 0, 0 };
	floor[0].position = { -50, -50, 100 };


	Object3D box;
	box.Init(obC, cam, 0);
	box.type = Object3D::Box;
	triangle.CreateModel("skydome");
	box.scale = { 1.0f, 1.0f, 1.0f };
	box.position = { 0,-10,0 };



	XMVECTOR boxQuaternion, quaternion2;

	boxQuaternion = XMQuaternionIdentity();//�P�ʃN�I�[�^�j�I���̐���
	quaternion2 = XMQuaternionIdentity();//�C�ӎ�(1,0,0)������90�x��]


	bool isTexture = false;
#pragma endregion
	//if (FAILED(result))
	//{
	//	return result;
	//}
#pragma endregion
	//�`�揉�������� �����܂�
	while (Win->loopBreak()) //�Q�[�����[�v
	{
		//�E�B���h�E���b�Z�[�W����
#pragma region WindowMessage
		Win->msgCheck();
#pragma endregion

		// DirectX���t���[������ ��������

#pragma region DirectX

		input->Update();
		//�L�[���͂̏������@
		//�o�b�N�o�b�t�@�̔ԍ����擾(2�K�v�Ȃ̂�0�Ԃ�1��)


		myDirectX->PreDraw();
		//�C�`��R�}���h��������
		if (input->Key(DIK_SPACE))
		{
			alarm.SoundPlayWave();
		}

#pragma endregion



		//�`�悱������

		if (input->Trigger(DIK_RIGHT) || input->Trigger(DIK_LEFT) || input->Trigger(DIK_UP) || input->Trigger(DIK_DOWN))
		{
			if (EQ.IsEnd())
			{
				EQ.Reset();
			}

			quaternion2 = XMQuaternionIdentity();
			if (input->Trigger(DIK_RIGHT))
			{
				box.rotation.y++;
				//quaternion2 = XMQuaternionRotationAxis(XMVECTOR{ 0, 0, 1, 0 }, XMConvertToRadians(-90));//�C�ӎ�(0,0,1)������90�x��]
			}
			if (input->Trigger(DIK_LEFT))
			{
				box.rotation.y--;
				//quaternion2 = XMQuaternionRotationAxis(XMVECTOR{ 0, 0, 1, 0 }, XMConvertToRadians(90));//�C�ӎ�(0,0,1)������90�x��]

			}
			if (input->Trigger(DIK_UP))
			{
				box.rotation.x++;
				//quaternion2 = XMQuaternionRotationAxis(XMVECTOR{ 1, 0, 0, 0 }, XMConvertToRadians(90));//�C�ӎ�(0,0,1)������90�x��]

			}
			if (input->Trigger(DIK_DOWN))
			{
				box.rotation.x--;
				//quaternion2 = XMQuaternionRotationAxis(XMVECTOR{ 1, 0, 0, 0 }, XMConvertToRadians(-90));//�C�ӎ�(0,0,1)������90�x��]
			}
			boxQuaternion = XMQuaternionMultiply(boxQuaternion, quaternion2);
		}


		if (input->Trigger(DIK_1))
		{
			isTexture = !isTexture;
		}
		if (EQ.IsEnd())
		{
			//boxQuaternion = XMQuaternionMultiply(boxQuaternion, quaternion2);
			//box.matWorld = XMMatrixRotationQuaternion(boxQuaternion);
		}
		else
		{
			//float rate = static_cast<float>(EQ.Linear());
			//XMVECTOR tmpQ = XMQuaternionMultiply(boxQuaternion, quaternion2);
			//XMVECTOR rateQ = XMQuaternionSlerp(boxQuaternion, tmpQ, rate);
			//box.matWorld = XMMatrixRotationQuaternion(rateQ);
			//if (EQ.IsEnd())
			//{
			//	//EQ.Reset();
			//	boxQuaternion = XMQuaternionMultiply(boxQuaternion, quaternion2);
			//}

		}
		box.SetConstBuffer(cam);


		for (int i = 0; i < 100; i++)
		{
			floor[i].Update( cam);
		}


		box.Update( cam);

		if (input->Key(DIK_A))
		{
			sprite.position.x--;
		}
		if (input->Key(DIK_D))
		{
			sprite.position.x++;
		}
		sprite.SpriteUpdate();



		//�R�}���h���X�g�ɒǉ�

#pragma region moveCamera
		if (input->Key(DIK_D) || input->Key(DIK_A))
		{
			if (input->Key(DIK_D))
			{
				angle += XMConvertToRadians(1.0f);
			}
			else if (input->Key(DIK_A))
			{
				angle -= XMConvertToRadians(1.0f);
			}
			cam.eye.x = -100 * sinf(angle);
			cam.eye.z = -100 * cosf(angle);
		}

		cam.Update();
#pragma endregion

		//�`��R�}���h

		//sprite.SpriteDraw();


		//debugText.Print("Hello,DirectX!!", 200, 100);

		//debugText.Print("abcdefghijklmnopqrstuvwxyz", 200, 200, 2.0f);

		//debugText.DrawAll();

		//�[�x�n���Z�b�g
		DepthReset();
		//bill.Draw(obC);
		//box.Draw(obC, Pipe3D->GetPipeLine(), test);

		box.modelDraw(triangle.GetModel(), model3D->GetPipeLine(), isTexture, test);
		//for (int i = 0; i < 100; i++)
		//{
		//	floor[i].Draw(obC, Pipe3D->GetPipeLine(), grid);
		//}

		//�C�`��R�}���h�����܂�

	//�D���\�[�X�o���A��߂�
		myDirectX->PostDraw();
		// DirectX���t���[������ �����܂�
	}
	//xAudio2���
	Sound::xAudioDelete();
	//�����f�[�^���
	alarm.SoundUnload();
	//�ǂ��Ń��������[�N���N�����Ă��邩���m�F���Ă��ꂽ
	//ID3D12DebugDevice *debugInterface;

	//if (SUCCEEDED(dev.Get()->QueryInterface(&debugInterface)))
	//{
	//	debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
	//	debugInterface->Release();
	//}
	//�E�B���h�E�N���X��o�^����
#pragma region WindowsAPI
	Win->end();
#pragma endregion
	return 0;
}


//SoundData SoundLoadWave(const char *filename)
//{
//	//HRESULT result;
//#pragma region fileopen
//	//�t�@�C�����̓X�g���[���̃C���X�^���X
//	std::ifstream file;
//	//wav�t�@�C�����o�C�i�����[�h�ŊJ��
//	file.open(filename, std::ios_base::binary);
//	//�t�@�C���I�[�v�����s�����o����
//	assert(file.is_open());
//#pragma endregion
//#pragma region wavRead
//	//RIFF�w�b�_�[�̓ǂݍ���
//	RiffHeader riff;
//	file.read((char *)&riff, sizeof(riff));
//	//�t�@�C����Riff���`�F�b�N
//	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
//	{
//		assert(0);
//	}
//	//���C�y��WAVE���`�F�b�N
//	if (strncmp(riff.type, "WAVE", 4) != 0)
//	{
//		assert(0);
//	}
//
//	//format�`�����N�̓ǂݍ���
//	FormatChunk format = {};
//	//�`�����N�w�b�_�̊m�F
//	file.read((char *)&format, sizeof(ChunkHeader));
//	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
//	{
//		assert(0);
//	}
//
//	//�`�����N�{�̂̓ǂݍ���
//	assert(format.chunk.size <= sizeof(format.fmt));
//	file.read((char *)&format.fmt, format.chunk.size);
//
//	//Data�`�����N�̓ǂݍ���
//	ChunkHeader data;
//	file.read((char *)&data, sizeof(data));
//	//JUNK�`�����N�����o�����ꍇ
//	if (strncmp(data.id, "JUNK", 4) == 0)
//	{
//		//�ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
//		file.seekg(data.size, std::ios_base::cur);
//		//�ēǂݍ���
//		file.read((char *)&data, sizeof(data));
//	}
//
//	if (strncmp(data.id, "data", 4) != 0)
//	{
//		assert(0);
//	}
//
//	//Data�`�����N�̃f�[�^��(�g�`�f�[�^)�̓ǂݍ���
//	char *pBuffer = new char[data.size];
//	file.read(pBuffer, data.size);
//
//
//#pragma endregion
//#pragma region fileClose
//	//WAVE�t�@�C�������
//	file.close();
//#pragma endregion
//#pragma region makeSound
//	//return����ׂ̂��񂹂��f�[�^
//	SoundData soundData = {};
//
//	soundData.wfex = format.fmt;
//	soundData.pBuffer = reinterpret_cast<BYTE *>(pBuffer);
//	soundData.bufferSize = data.size;
//#pragma endregion
//
//	return soundData;
//}
//
//void SoundUnload(SoundData *soundData)
//{
//	//�o�b�t�@�̃����������
//	delete[] soundData->pBuffer;
//
//	soundData->pBuffer = 0;
//	soundData->bufferSize = 0;
//	soundData->wfex = {};
//}
//
//void SoundPlayWave(IXAudio2 *xAudio2, const SoundData &soundData)
//{
//	{
//		HRESULT result;
//
//		//�g�`�t�H�[�}�b�g�����Ƃ�SourceVol�ł̐���
//		IXAudio2SourceVoice *pSourceVoice = nullptr;
//		result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
//		assert(SUCCEEDED(result));
//
//		//�Đ�����g�`�f�[�^�̐ݒ�
//		XAUDIO2_BUFFER buf{};
//		buf.pAudioData = soundData.pBuffer;
//		buf.AudioBytes = soundData.bufferSize;
//		buf.Flags = XAUDIO2_END_OF_STREAM;
//
//		//�g�`�f�[�^�̍Đ�
//		result = pSourceVoice->SubmitSourceBuffer(&buf);
//		result = pSourceVoice->Start();
//	}
//
//}

