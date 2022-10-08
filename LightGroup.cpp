#include "LightGroup.h"
#include "MyDirectX.h"

using namespace DirectX;
LightGroup::LightGroup()
{
	HRESULT result;
	//定数バッファの生成
	result = MyDirectX::Instance()->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	constBuff->SetName(L"lightBuff");
	if (FAILED(result)) assert(0);
}

void LightGroup::Init()
{
	TransferConstBuffer();
}

void LightGroup::Update()
{
	bool isLight = false;
	bool isPointLight = false;
	bool isSpotLight = false;
	if (light)
	{
		isLight = light->GetDirty();
	}
	if (pointLight)
	{
		isPointLight = pointLight->GetDirty();
	}
	if (spotLight)
	{
		isSpotLight = spotLight->IsDirty();
	}
	if (dirty || isLight || isPointLight || isSpotLight)
	{
		TransferConstBuffer();
		dirty = false;
		if(isLight)
		{
			light->SetDirty(false);
		}
		if (isPointLight)
		{
			pointLight->SetDirty(false);
		}
		if (isSpotLight)
		{
			spotLight->SetDirty(false);
		}
	}
}

void LightGroup::Draw(UINT rootParameterIndex)
{
	//定数バッファビューをセット
	MyDirectX::Instance()->GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff->GetGPUVirtualAddress());
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;
	//定数バッファへデータ転送
	ConstBufferData *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void **)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->ambientColor = ambientColor;

		//平行高原が設定されていなかったら
		if (light == nullptr)
		{
			constMap->dirLight.lightv = -XMVECTOR{ 1, 1, 1, 0 };
			constMap->dirLight.lightcolor = XMFLOAT3{ 1, 1, 1 };
			constMap->dirLight.active = 1;
		}
		else
		{
			constMap->dirLight.lightv = -light->GetLightDir();
			constMap->dirLight.lightcolor = light->GetLightColor();
			if (light->GetLightActive())
			{
				constMap->dirLight.active = 1;
			}
			else
			{
				constMap->dirLight.active = 0;
			}
		}


		if (pointLight == nullptr)
		{
			constMap->pointLight.lightcolor = {1, 1, 1};
			constMap->pointLight.lightpos = { 0, 0, 0 };
			constMap->pointLight.lightatten = {1.0f,1.0f ,1.0f };
			constMap->pointLight.active = 0;
		}
		else
		{
			if (pointLight->IsActive())
			{
				constMap->pointLight.lightcolor = pointLight->GetLightColor();
				constMap->pointLight.lightpos = pointLight->GetlightPos();
				constMap->pointLight.lightatten = pointLight->GetLightAtten();
				constMap->pointLight.active = 1;
			}
			else
			{
				constMap->pointLight.active = 0;
			}

		}

		if (spotLight == nullptr)
		{
			constMap->spotLight.active = 0;
			constMap->spotLight.lightv = -XMVECTOR{1, 1, 0};
			constMap->spotLight.lightpos = XMFLOAT3{0, 0, 0};
			constMap->spotLight.lightcolor = XMFLOAT3{ 0, 0, 0 };
			constMap->spotLight.lightatten = XMFLOAT3{ 0, 0, 0 };
			constMap->spotLight.lightfactoranglecos = XMFLOAT2{ 0.5f, 0.2f };
		}
		else
		{
			if (spotLight->IsActive())
			{
				constMap->spotLight.active = 1;
				constMap->spotLight.lightv = -spotLight->GetLightDir();
				constMap->spotLight.lightpos = spotLight->GetLightPos();
				constMap->spotLight.lightcolor = spotLight->GetLightColor();
				constMap->spotLight.lightatten = spotLight->GetLightAtten();
				constMap->spotLight.lightfactoranglecos = spotLight->GetLightFactorAngleCos();
			}
			else
			{
				constMap->spotLight.active = 0;
			}
		}

		constBuff->Unmap(0, nullptr);
	}
}

LightGroup *LightGroup::Create()
{
	LightGroup *obj = new LightGroup();

	if (obj)
	{
		obj->Init();
	}
	return obj;
}

void LightGroup::SetLight(Light *light)
{
	this->light = light;
	dirty = true;
}

void LightGroup::SetPoinntLight(PointLight *pointLight)
{
	this->pointLight = pointLight;
	dirty = true;
}

void LightGroup::SetSpotLight(SpotLight *spotLight)
{
	this->spotLight = spotLight;
}

void LightGroup::SetAmbientColor(const XMFLOAT3 &ambientColor)
{
	this->ambientColor = ambientColor;
	dirty = true;
}