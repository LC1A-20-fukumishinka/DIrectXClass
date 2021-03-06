#include "Light.h"
#include "MyDirectX.h"
using namespace DirectX;

ID3D12Device *Light::device = nullptr;
void Light::StaticInitialize(ID3D12Device *device)
{
	//再初期化チェック
	assert(!Light::device);
	//nullptr
	assert(device);
	//静的メンバ変数のセット
	Light::device = device;
}
Light *Light::Create()
{
	//3Dオブジェクトのインスタンスを生成
	Light *instance = new Light();
	//初期化
	instance->Initialize();
	//生成下インスタンスを返す
	return instance;
}
Light::Light()
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

Light::~Light()
{
}

void Light::Initialize()
{
		//定数バッファデータ転送
		TransferConstBuffer();
}

void Light::Update()
{
//値の更新があった時だけ定数バッファに転送する
	if (dirty)
	{
		TransferConstBuffer();
		dirty = false;
	}
}

void Light::Draw( UINT rootParameterIndex)
{
	//定数バッファビューをセット
	MyDirectX::Instance()->GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex, 
	constBuff->GetGPUVirtualAddress());
}

void Light::TransferConstBuffer()
{
	HRESULT result;
	//定数バッファへデータ転送
	ConstBufferData *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->lightv = -lightdir;
		constMap-> lightcolor = lightcolor;
		constMap->active = isActive;
		constBuff->Unmap(0, nullptr);
	}
}

void Light::SetLightDir(const XMVECTOR &lightdir)
{
	//正規化してセット
	this -> lightdir = XMVector3Normalize(lightdir);
	dirty = true;
}

void Light::SetLightColor(const XMFLOAT3 &lightcolor)
{
	this->lightcolor = lightcolor;
	dirty = true;
}
