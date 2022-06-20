#pragma once
#include <DirectXMath.h>
namespace GameInput
{
	bool ATrigger();

	bool A();

	DirectX::XMFLOAT2 LStick(bool YReverse = false, bool XReverse = false);

	DirectX::XMFLOAT2 RStick(bool YReverse = true, bool XReverse = false);

	bool BTrigger();

	bool GrabInput();

	bool LockOnInput();

	bool Pause();
}

