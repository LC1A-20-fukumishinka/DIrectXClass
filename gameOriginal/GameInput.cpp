#include "GameInput.h"
#include "DirectInput.h"
bool GameInput::ATrigger()
{
	return Input::Instance()->KeyTrigger(DIK_Z) || Input::Instance()->ButtonTrigger(XINPUT_GAMEPAD_A);
}

bool GameInput::A()
{
	return Input::Instance()->Key(DIK_Z) || Input::Instance()->Button(XINPUT_GAMEPAD_A);
}

DirectX::XMFLOAT2 GameInput::LStick()
{
	return Input::Instance()->LStick();
}

DirectX::XMFLOAT2 GameInput::RStick()
{
	return Input::Instance()->RStick();
}

bool GameInput::BTrigger()
{
	return Input::Instance()->KeyTrigger(DIK_X) || Input::Instance()->ButtonTrigger(XINPUT_GAMEPAD_B);
}

bool GameInput::B()
{
	return Input::Instance()->Key(DIK_X) || Input::Instance()->Button(XINPUT_GAMEPAD_B);
}

bool GameInput::Pause()
{
	return Input::Instance()->KeyTrigger(DIK_Q) || Input::Instance()->ButtonTrigger(XINPUT_GAMEPAD_START);
}
