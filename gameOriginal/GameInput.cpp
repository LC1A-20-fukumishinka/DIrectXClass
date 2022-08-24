#include "GameInput.h"
#include "DirectInput.h"
bool GameInput::isMakeInstance = false;
GameInput::GameInput()
{
}
GameInput::~GameInput()
{
}
std::unique_ptr<GameInput> &GameInput::Instance()
{
	static std::unique_ptr<GameInput> instance;
	if (!isMakeInstance)
	{
		GameInput *makeInstance = new(GameInput);
		instance = std::unique_ptr<GameInput>(makeInstance);
		isMakeInstance = true;
	}
	return instance;
}
void GameInput::Update()
{
	if (isControl_)
	{
		isOldGrab = isGrab;
		isOldLockOn = isLockOn;
		isGrab = Input::Instance()->Key(DIK_X) || Input::Instance()->RTrigger();
		isLockOn = Input::Instance()->Key(DIK_X) || Input::Instance()->LTrigger();
	}
}
bool GameInput::ATrigger()
{
	return Input::Instance()->KeyTrigger(DIK_Z) || Input::Instance()->ButtonTrigger(XINPUT_GAMEPAD_A);
}

bool GameInput::A()
{
	return Input::Instance()->Key(DIK_Z) || Input::Instance()->Button(XINPUT_GAMEPAD_A) && isControl_;
}

bool GameInput::X()
{
	return Input::Instance()->Key(DIK_C) || Input::Instance()->Button(XINPUT_GAMEPAD_X) && isControl_;
}

bool GameInput::XTrigger()
{
	return Input::Instance()->KeyTrigger(DIK_C) || Input::Instance()->ButtonTrigger(XINPUT_GAMEPAD_X) && isControl_;
}

DirectX::XMFLOAT2 GameInput::LStick(bool YReverse, bool XReverse)
{
	DirectX::XMFLOAT2 output = Input::Instance()->LStick();
	if (!isControl_)
	{
		output = DirectX::XMFLOAT2();
	}
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

	if (!isControl_)
	{
		output = DirectX::XMFLOAT2();
	}
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

bool GameInput::B()
{
	return Input::Instance()->KeyTrigger(DIK_X) || Input::Instance()->Button(XINPUT_GAMEPAD_B) && isControl_;
}

bool GameInput::BTrigger()
{
	return Input::Instance()->KeyTrigger(DIK_X) || Input::Instance()->ButtonTrigger(XINPUT_GAMEPAD_B) && isControl_;
}

bool GameInput::GrabInput()
{
	return isGrab && isControl_;
}

bool GameInput::GrabTrigger()
{
	return (isGrab && !isOldGrab) && isControl_;
}

bool GameInput::GrabRelease()
{
	return (!isGrab && isOldGrab) && isControl_;
}

bool GameInput::LockOnInput()
{
	return isLockOn && isControl_;
}

bool GameInput::LockOnTrigger()
{
	return (isLockOn && !isOldLockOn) && isControl_;
}

bool GameInput::LockOnRelease()
{
	return (!isLockOn && isOldLockOn) && isControl_;
}

bool GameInput::Pause()
{
	return Input::Instance()->KeyTrigger(DIK_Q) || Input::Instance()->ButtonTrigger(XINPUT_GAMEPAD_START) && isControl_;
}

void GameInput::SetIsControll(bool ControllFlag)
{
	isControl_ = ControllFlag;
}
