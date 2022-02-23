#include "SpotLight.h"

SpotLight *SpotLight::Create()
{
	SpotLight *spotLight = nullptr;
	spotLight = new SpotLight;
	
	if (spotLight)
	{
		return spotLight;
	}
	return nullptr;
}

void SpotLight::SetLightDir(const XMVECTOR &lightdir)
{
	this->lightdir = DirectX::XMVector3Normalize(lightdir);
	dirty = true;
}

void SpotLight::SetLightPos(const XMFLOAT3 &lightpos)
{
	this->lightpos = lightpos;
	dirty = true;
}

void SpotLight::SetLightColor(const XMFLOAT3 &lightcolor)
{
	this->lightcolor = lightcolor;
	dirty = true;
}

void SpotLight::SetLightAtten(const XMFLOAT3 &lightAtten)
{
	this->lightAtten = lightAtten;
	dirty = true;
}

void SpotLight::SetLightFactorAngle(const XMFLOAT2 &lightFactorAngle)
{
	this->lightFactorAngleCos.x = cosf(DirectX::XMConvertToRadians(lightFactorAngle.x));
	this->lightFactorAngleCos.y = cosf(DirectX::XMConvertToRadians(lightFactorAngle.y));
	dirty = true;
}
