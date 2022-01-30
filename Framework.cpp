#include "Framework.h"
#include "BaseData.h"

void Framework::Init()
{
	Win = WinAPI::GetInstance();
	Win->Init(WINDOW_WIDTH, WINDOW_HEIGHT);


	myDirectX = MyDirectX::GetInstance();
	myDirectX->Init();

	Sound::StaticInitialize();
	ParticleManager::StaticInitialize();

	debugText = DebugText::Create();

}

void Framework::Finalize()
{
	delete debugText;
	//xAudio2解放
	Sound::xAudioDelete();
	//音声データ解放
	Sound::SoundUnload();
	Win->end();
}
