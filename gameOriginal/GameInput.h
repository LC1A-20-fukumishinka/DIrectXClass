#pragma once
#include <DirectXMath.h>
#include <memory>
class GameInput
{
	GameInput();
public:
	GameInput(const GameInput &obj) = delete;
	GameInput operator=(const GameInput &obj) = delete;
	static std::unique_ptr<GameInput> &Instance();
public:
	~GameInput();

	void Update();

	bool A();
	bool ATrigger();

	bool B();
	bool BTrigger();

	bool X();
	bool XTrigger();
	DirectX::XMFLOAT2 LStick(bool YReverse = false, bool XReverse = false);

	DirectX::XMFLOAT2 RStick(bool YReverse = true, bool XReverse = false);


	bool GrabInput();
	bool GrabTrigger();
	bool GrabRelease();

	bool LockOnInput();
	bool LockOnTrigger();
	bool LockOnRelease();

	bool CameraReflesh();
	bool Pause();

	/// <summary>
	/// 操作全部オンオフ
	/// </summary>
	/// <param name="ControllFlag">フラグ</param>
	void SetIsControll(bool ControllFlag);
private:
	static bool isMakeInstance;
private:
	bool isGrab = false;
	bool isOldGrab = false;
	bool isLockOn = false;
	bool isOldLockOn = false;
	bool isControl_ = true;
};

