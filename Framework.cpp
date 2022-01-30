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
