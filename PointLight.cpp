#include "PointLight.h"

using namespace DirectX;
PointLight *PointLight::Create()
{
	PointLight *pointLight = nullptr;
	pointLight = new PointLight;

	if (pointLight)
	{
		return pointLight;
	}
	return nullptr;
}

void PointLight::SetLightPos(const XMFLOAT3 &lightpos)
{
	this->lightPos = lightpos;
	dirty = true;
}

void PointLight::SetLightColor(const XMFLOAT3 &lightcolor)
{
	this->lightColor = lightcolor;
	dirty = true;
}

void PointLight::SetLightAtten(const XMFLOAT3 &lightAtten)
{
	this->lightAtten = lightAtten;
	dirty = true;
}

void PointLight::SetActive(bool active)
{
	this->active = active;
	dirty = true;
}

void PointLight::SetDirty(bool dirty)
{
	this->dirty = dirty;
}
