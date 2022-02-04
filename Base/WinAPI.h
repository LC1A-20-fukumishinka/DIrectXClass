#pragma once
#include <Windows.h>
class WinAPI
{
private:
	WinAPI();
	~WinAPI();
public:
	WinAPI &operator=(const WinAPI &obj) = delete;
	WinAPI(const WinAPI &obj) = delete;

	WNDCLASSEX w;//�E�B���h�E�N���X�̐ݒ�
	MSG msg;//���b�Z�[�W
	HWND hwnd;

	int winWidth;
	int winHeight;

	static WinAPI *Instance();

	void Init(const int winWidth, const int winHeight);

	void msgCheck();

	bool loopBreak();

	void end();

};

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
