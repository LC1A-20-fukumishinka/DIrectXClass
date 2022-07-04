#include "EaseClass.h"
#include <math.h>
#include <algorithm>



Easing::Easing()
{
	maxTimer = 1;
	timer = 0;
	rate = 0.0;
}

float Easing::EaseCalc(int InOrOut, int type, float t)
{
	t = std::clamp<float>(t,0.0f, 1.0f);
	 
	switch (InOrOut)
	{
	case In:
		switch (type)
		{
		case Sine:
			return easeInSine(t);
			break;
		case Quad:
			return easeInQuad(t);
			break;
		case Cubic:
			return easeInCubic(t);
			break;
		case Quart:
			return easeInQuart(t);
			break;
		case Quint:
			return easeInQuint(t);
			break;
		case Expo:
			return easeInExpo(t);
			break;
		case Circ:
			return easeInCirc(t);
			break;
		case Back:
			return easeInBack(t);
			break;
		case Elastic:
			return easeInElastic(t);
			break;
		case Bounce:
			return easeInBounce(t);
			break;
		default:
			break;
		}
		break;
	case Out:
		switch (type)
		{
		case Sine:
			return easeOutSine(t);
			break;
		case Quad:
			return easeOutQuad(t);
			break;
		case Cubic:
			return easeOutCubic(t);
			break;
		case Quart:
			return easeOutQuart(t);
			break;
		case Quint:
			return easeOutQuint(t);
			break;
		case Expo:
			return easeOutExpo(t);
			break;
		case Circ:
			return easeOutCirc(t);
			break;
		case Back:
			return easeOutBack(t);
			break;
		case Elastic:
			return easeOutElastic(t);
			break;
		case Bounce:
			return easeOutBounce(t);
			break;
		default:
			break;
		}
		break;
	case InOut:
		switch (type)
		{
		case Sine:
			return easeinOutSine(t);
			break;
		case Quad:
			return easeInOutQuad(t);
			break;
		case Cubic:
			return easeInOutCubic(t);
			break;
		case Quart:
			return easeInOutQuart(t);
			break;
		case Quint:
			return easeInOutQuint(t);
			break;
		case Expo:
			return easeInOutExpo(t);
			break;
		case Circ:
			return easeInOutCirc(t);
			break;
		case Back:
			return easeInOutBack(t);
			break;
		case Elastic:
			return easeInOutElastic(t);
			break;
		case Bounce:
			return easeInOutBounce(t);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return 0.0;
}

float Easing::Do(int InOrOut, int type)
{
	timer++;
	if (timer >= maxTimer) return 1.0;

	float t = static_cast<float>(timer) / maxTimer;

	rate = EaseCalc(InOrOut, type, t);
	return rate;
}

float Easing::Linear()
{
	if (timer >= maxTimer) return 1.0;

	timer += 1;

	rate = static_cast<float>(timer) / maxTimer;
	return rate;
}

void Easing::Init(int max)
{
	maxTimer = max;
	timer = 0;
}

bool Easing::IsEnd()
{
	if (timer < maxTimer)	return false;
	return true;
}

float Easing::Read()
{
	return rate;
}

void Easing::Reset()
{
	timer = 0;
	rate = 0.0;
}

//Sine(1)
//easeInSine(ゆっくり→速い)
float Easing::easeInSine(const float rate)
{
	//変化率
	float changeRate;
	float PI = 3.14159265f;

	changeRate = rate;
	return 1 - cosf((changeRate * PI) / 2);
}

//easeOutSine(速い→ゆっくり)
float Easing::easeOutSine(const float rate)
{
	//変化率
	float changeRate;
	float PI = 3.14159265f;

	changeRate = rate;
	return sinf((changeRate * PI) / 2);

}

//easeInOutSine(ゆっくり→速い→ゆっくり)
float Easing::easeinOutSine(const float rate)
{
	//変化率
	float changeRate;
	float PI = 3.14159265f;

	changeRate = rate;
	return -(cosf(PI * changeRate) - 1) / 2;

}


//Quad(2乗)
//easeInQuad(ゆっくり→速い)
float Easing::easeInQuad(const float rate)
{
	//変化率
	float changeRate;
	//ヌルチェック済み

	changeRate = rate;
	return changeRate * changeRate;

}

//easeOutQuad(速い→ゆっくり)
float Easing::easeOutQuad(const float rate)
{
	//変化率
	float changeRate;
	changeRate = rate;
	return 1 - (1 - changeRate) * (1 - changeRate);

}

//easeInOutQuad(ゆっくり→速い→ゆっくり)
float Easing::easeInOutQuad(const float rate)
{
	//変化率
	float changeRate;
	changeRate = rate;
	return				 changeRate < 0.5f ?
		2 * changeRate * changeRate : 1 - powf(-2 * changeRate + 2, 2) / 2;
}


//Cubic(3乗)
//easeInCubic(ゆっくり→速い)
float Easing::easeInCubic(const float rate)
{
	//変化率
	float changeRate;
	//ヌルチェック済み

	changeRate = rate;
	return changeRate * changeRate * changeRate;

}

//easeOutCubic(速い→ゆっくり)
float Easing::easeOutCubic(const float rate)
{
	//変化率
	float changeRate;

	changeRate = rate;
	return 1 - powf(1 - changeRate, 3);

}

//easeInOutCubic(ゆっくり→速い→ゆっくり)
float Easing::easeInOutCubic(const float rate)
{
	//変化率
	float changeRate;

	changeRate = rate;
	return							  changeRate < 0.5 ?
		4 * changeRate * changeRate * changeRate : 1 - powf(-2 * changeRate + 2, 3) / 2;

}


//Quart(4乗)
//easeInQuart(ゆっくり→速い)
float Easing::easeInQuart(const float rate)
{
	//変化率
	float changeRate;

	changeRate = rate;
	return changeRate * changeRate * changeRate * changeRate;

}

//easeOutQuart(速い→ゆっくり)
float Easing::easeOutQuart(const float rate)
{
	//変化率
	float changeRate;

	changeRate = rate;
	return 1 - powf(1 - changeRate, 4);

}

//easeInOutQuart(ゆっくり→速い→ゆっくり)
float Easing::easeInOutQuart(const float rate)
{
	//変化率
	float changeRate;
	//ヌルチェック済み

	changeRate = rate;
	return										   changeRate < 0.5 ?
		8 * changeRate * changeRate * changeRate * changeRate : 1 - powf(-2 * changeRate + 2, 4) / 2;

}


//Quint(5乗)
//easeInQuint(ゆっくり→速い)
float Easing::easeInQuint(const float rate)
{
	//変化率
	float changeRate;

	changeRate = rate;
	return changeRate * changeRate * changeRate * changeRate * changeRate;

}

//easeOutQuint(速い→ゆっくり)
float Easing::easeOutQuint(const float rate)
{
	//変化率
	float changeRate;

	changeRate = rate;
	return 1 - powf(1 - changeRate, 5);

}

//easeInOutQuint(ゆっくり→速い→ゆっくり)
float Easing::easeInOutQuint(const float rate)
{
	//変化率
	float changeRate;
	//ヌルチェック済み

	changeRate = rate;
	return														 changeRate < 0.5 ?
		16 * changeRate * changeRate * changeRate * changeRate * changeRate : 1 - powf(-2 * changeRate + 2, 5) / 2;

}


//Expo(急カーブ)
//easeInExpo
float Easing::easeInExpo(const float rate)
{
	//変化率
	float changeRate;

	changeRate = rate;
	return changeRate == 0 ? 0 : powf(2, 10 * changeRate - 10);

}

//easeOutExpo
float Easing::easeOutExpo(const float rate)
{
	//変化率
	float changeRate;

	changeRate = rate;
	return changeRate == 1 ? 1 : 1 - powf(2, -10 * changeRate);

}

//easeInOutExpo
float Easing::easeInOutExpo(const float rate)
{
	//変化率
	float changeRate;

	changeRate = rate;
	return						changeRate == 0 ?
		0 : changeRate == 1 ?
		1 : changeRate < 0.5 ?
		powf(2, 20 * changeRate - 10) / 2 : (2 - powf(2, -20 * changeRate + 10)) / 2;

}


//Circ
//easeInCirc
float Easing::easeInCirc(const float rate)
{
	//変化率
	float changeRate;

	changeRate = rate;
	return 1 - sqrtf(1 - powf(changeRate, 2));

}

//easeOutCirc
float Easing::easeOutCirc(const float rate)
{
	//変化率
	float changeRate;

	changeRate = rate;
	return sqrtf(1 - powf(changeRate - 1, 2));

}

//easeInOutCirc
float Easing::easeInOutCirc(const float rate)
{
	//変化率
	float changeRate;

	changeRate = rate;
	return changeRate < 0.5f
		? (1 - sqrtf(1 - powf(2 * changeRate, 2))) / 2
		: (sqrtf(1 - powf(-2 * changeRate + 2, 2)) + 1) / 2;

}


//Back
//easeInBack
float Easing::easeInBack(const float rate)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;
	//変化率
	float changeRate;

	changeRate = rate;
	return c3 * changeRate * changeRate * changeRate - c1 * changeRate * changeRate;

}

//easeOutBack
float Easing::easeOutBack(const float rate)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;
	//変化率
	float changeRate;

	changeRate = rate;
	return 1 + c3 * powf(changeRate - 1, 3) + c1 * powf(changeRate - 1, 2);

}

//easeInOutBack
float Easing::easeInOutBack(const float rate)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;
	//変化率
	float changeRate;

	changeRate = rate;
	return changeRate < 0.5
		? (powf(2 * changeRate, 2) * ((c2 + 1) * 2 * changeRate - c2)) / 2
		: (powf(2 * changeRate - 2, 2) * ((c2 + 1) * (changeRate * 2 - 2) + c2) + 2) / 2;

}


//Elastic
//easeInElastic
float Easing::easeInElastic(const float rate)
{
	float PI = 3.14159265f;
	const float c4 = (2 * PI) / 3;
	//変化率
	float changeRate;

	changeRate = rate;
	return changeRate == 0
		? 0
		: changeRate == 1
		? 1
		: -powf(2, 10 * changeRate - 10) * sinf((changeRate * 10 - 10.75f) * c4);

}

//easeOutElastic
float Easing::easeOutElastic(const float rate)
{
	float PI = 3.14159265f;
	const float c4 = (2 * PI) / 3;
	//変化率
	float changeRate;

	changeRate = rate;
	return changeRate == 0
		? 0
		: changeRate == 1
		? 1
		: -powf(2, -10 * changeRate) * sinf((changeRate * 10 - 0.75f) * c4) + 1;

}

//easeInOutElastic
float Easing::easeInOutElastic(const float rate)
{
	float PI = 3.14159265f;
	const float c5 = (2 * PI) / 4.5f;
	//変化率
	float changeRate;

	changeRate = rate;
	return changeRate == 0
		? 0
		: changeRate == 1
		? 1
		: changeRate < 0.5
		? -(powf(2, 20 * changeRate - 10) * sinf((20 * changeRate - 11.125f) * c5)) / 2
		: (powf(2, -20 * changeRate + 10) * sinf((20 * changeRate - 11.125f) * c5)) / 2 + 1;

}


//Bounce
//easeInBounce
float Easing::easeInBounce(const float rate)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;
	//変化率
	float changeRate;

	changeRate = 1.0f - (rate);
	if (changeRate < 1.0f / d1)
	{
		return 1 - (n1 * changeRate * changeRate);
	}
	else if (changeRate < 2.0f / d1)
	{
		return 1 - (n1 * (changeRate -= 1.5f / d1) * changeRate + 0.75f);//0.11
	}
	else if (changeRate < 2.5f / d1)
	{
		return 1 - (n1 * (changeRate -= 2.25f / d1) * changeRate + 0.9375f);//0.1111
	}
	else
	{
		return 1 - (n1 * (changeRate -= 2.625f / d1) * changeRate + 0.984375f);//0.111111
	}

}

//easeOutBounce
float Easing::easeOutBounce(const float rate)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;
	//変化率
	float changeRate;

	changeRate = rate;
	if (changeRate < 1.0f / d1)
	{
		return n1 * changeRate * changeRate;
	}
	else if (changeRate < 2.0f / d1)
	{
		return n1 * (changeRate -= 1.5f / d1) * changeRate + 0.75f;//0.11
	}
	else if (changeRate < 2.5f / d1)
	{
		return n1 * (changeRate -= 2.25f / d1) * changeRate + 0.9375f;//0.1111
	}
	else
	{
		return n1 * (changeRate -= 2.625f / d1) * changeRate + 0.984375f;//0.111111
	}

}

//easeInOutBounce
float Easing::easeInOutBounce(const float rate)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;
	//変化率
	float changeRate;

	changeRate = rate;
	if (changeRate < 0.5f)
	{
		changeRate = (1 - 2 * changeRate);
		if (changeRate < 1 / d1)
		{
			return (1 - (n1 * changeRate * changeRate)) / 2;
		}
		else if (changeRate < 2 / d1)
		{
			return (1 - (n1 * (changeRate -= 1.5f / d1) * changeRate + 0.75f)) / 2;//0.11
		}
		else if (changeRate < 2.5f / d1)
		{
			return (1 - (n1 * (changeRate -= 2.25f / d1) * changeRate + 0.9375f)) / 2;//0.1111
		}
		else
		{
			return (1 - (n1 * (changeRate -= 2.625f / d1) * changeRate + 0.984375f)) / 2;//0.111111
		}
	}
	else
	{
		changeRate = (2 * changeRate - 1);
		if (changeRate < 1 / d1)
		{
			return (1 + (n1 * changeRate * changeRate)) / 2;
		}
		else if (changeRate < 2 / d1)
		{
			return (1 + (n1 * (changeRate -= 1.5f / d1) * changeRate + 0.75f)) / 2;//0.11
		}
		else if (changeRate < 2.5f / d1)
		{
			return (1 + (n1 * (changeRate -= 2.25f / d1) * changeRate + 0.9375f)) / 2;//0.1111
		}
		else
		{
			return (1 + (n1 * (changeRate -= 2.625f / d1) * changeRate + 0.984375f)) / 2;//0.111111
		}
	}

}