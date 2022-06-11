#pragma once
#include <DirectXMath.h>
namespace GameInput
{
	bool ATrigger();

	bool A();

	DirectX::XMFLOAT2 LStick();

	DirectX::XMFLOAT2 RStick();

	bool BTrigger();

	bool B();

	bool Pause();
}

