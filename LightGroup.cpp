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
	if (dirty || light->GetDirty())
	{
		TransferConstBuffer();
		dirty = false;
		light->SetDirty(false);
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

void LightGroup::SetAmbientColor(XMFLOAT3 ambientColor)
{
	this->ambientColor = ambientColor;
	dirty = true;
}