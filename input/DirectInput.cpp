#include "DirectInput.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::Init(WNDCLASSEX &w, HWND &hwnd)
{
	for (int i = 0; i < 256; i++)
	{
		key[i] = 0;
		oldkey[i] = 0;
	}

	result = DirectInput8Create(
		w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dinput, nullptr);

	devkeyboard = nullptr;
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);

	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);	//�W���`��

	result = devkeyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

}

void Input::Update()
{
	for (int i = 0; i < 256; i++)
	{
		oldkey[i] = key[i];
	}
	HRESULT result;
	result = devkeyboard->Acquire();//���͎󂯕t���܂���̏���

	result = devkeyboard->GetDeviceState(sizeof(key), key);//�S���̃L�[���͂̏�Ԃ��擾����

}

bool Input::Key(int KeyCode)
{
	if (key[KeyCode] == 0x80)return true;
	return false;
}

bool Input::Trigger(int KeyCode)
{
	if ((key[KeyCode] == 0x80) && (oldkey[KeyCode] == 0x00))return true;
	return false;
}

bool Input::Release(int KeyCode)
{
	if ((key[KeyCode] == 0x00) && (oldkey[KeyCode] == 0x80))return true;
	return false;
}

Input *Input::GetInstance()
{
	static Input instance;
	return &instance;
}
