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

DirectX::XMFLOAT2 GameInput::LStick(bool YReverse, bool XReverse)
{
	DirectX::XMFLOAT2 output = Input::Instance()->LStick();
	if (YReverse)
	{
		output.y = -output.y;
	}
	if (XReverse)
	{
		output.x = -output.x;
	}
	return output;
}

DirectX::XMFLOAT2 GameInput::RStick(bool YReverse, bool XReverse)
{
	DirectX::XMFLOAT2 output = Input::Instance()->RStick();
	if (YReverse)
	{
		output.y = -output.y;
	}
	if (XReverse)
	{
		output.x = -output.x;
	}
	return output;
}

bool GameInput::BTrigger()
{
	return Input::Instance()->KeyTrigger(DIK_X) || Input::Instance()->ButtonTrigger(XINPUT_GAMEPAD_B);
}

bool GameInput::GrabInput()
{
	return Input::Instance()->Key(DIK_X) || Input::Instance()->RTrigger();
}

bool GameInput::LockOnInput()
{
	return Input::Instance()->Key(DIK_X) || Input::Instance()->LTrigger();
}

bool GameInput::Pause()
{
	return Input::Instance()->KeyTrigger(DIK_Q) || Input::Instance()->ButtonTrigger(XINPUT_GAMEPAD_START);
}
