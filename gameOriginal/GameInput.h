#pragma once
#include <DirectXMath.h>
#include <memory>
class GameInput
{
public:
	GameInput(const GameInput &obj) = delete;
	GameInput operator=(const GameInput &obj) = delete;
	static std::unique_ptr<GameInput> &Instance();
public:
	GameInput();
	~GameInput();

	void Update();
	bool ATrigger();

	bool A();

	DirectX::XMFLOAT2 LStick(bool YReverse = false, bool XReverse = false);

	DirectX::XMFLOAT2 RStick(bool YReverse = true, bool XReverse = false);

	bool BTrigger();

	bool GrabInput();
	bool GrabTrigger();
	bool GrabRelease();

	bool LockOnInput();
	bool LockOnTrigger();
	bool LockOnRelease();

	bool Pause();
private:
	static bool isMakeInstance;
private:
	bool isGrab = false;
	bool isOldGrab = false;
	bool isLockOn = false;
	bool isOldLockOn = false;
};

