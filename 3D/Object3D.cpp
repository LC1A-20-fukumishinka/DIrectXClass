#include "Object3D.h"
#include "TextureMgr.h"
#include "Model.h"
#include "../Collision/BaseCollider.h"
#include "../Collision/CollisionMgr.h"
#include "FukuMath.h"
using namespace DirectX;
using namespace FukuMath;

Object3D::~Object3D()
{
	if (collider)
	{
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}
}

void Object3D::Init()
{
	HRESULT result = S_FALSE;
	MyDirectX *myD = MyDirectX::Instance();
	//ワールド行列を設定する

	quaternion = XMQuaternionIdentity();
	matWorld = XMMatrixIdentity();

	matWorld = GetMatWorld();


	//定数バッファの生成
	if (!isMakeConstBuffer)
	{
		HRESULT result = S_FALSE;
		MyDirectX *myD = MyDirectX::Instance();

		result = myD->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff)
		);

		constBuff->SetName(L"ConstBuff3D");
		if (result)
		{
			assert(0);
		}
		isMakeConstBuffer = true;
	}

	//クラス名の文字列を取得
	name = typeid(*this).name();
}

const XMMATRIX Object3D::GetMatWorld()
{
	XMMATRIX matScale, matRot, matTrans, matTmp;


	//各種アフィン変換を行列の形にする
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	//matRot *= XMMatrixRotationZ(rotation.z);
	//matRot *= XMMatrixRotationX(rotation.x);
	//matRot *= XMMatrixRotationY(rotation.y);
	matRot = XMMatrixRotationQuaternion(quaternion);
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//オブジェクトの軸のベクトルを更新

	//正面ベクトルを再計算
	UpdateVector();

	//各種変換行列を乗算してゆく
	matTmp = XMMatrixIdentity();
	matTmp *= matScale;
	matTmp *= matRot;
	matTmp *= matTrans;

	//もし親オブジェクトがあるなら
	if (parent != nullptr)
	{
		//親オブジェクトのワールド行列を乗算する
		matTmp *= parent->GetMatWorld();
	}

	//生成された行列を返す
	return matTmp;
}

void Object3D::Update()
{
	//ワールド行列を設定する
	matWorld = GetMatWorld();

	if (camera == nullptr)
	{
		assert(0);
	}

	buffData.cameraPos = camera->GetEye();
	buffData.color = color;
	buffData.viewproj = camera->GetMatViewProj();
	buffData.world = matWorld;

	void *constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, &constMap);
	std::memcpy(constMap, &buffData, sizeof(buffData));
	constBuff->Unmap(0, nullptr);

	if (collider)
	{
		collider->Update();
	}
}


void Object3D::modelDraw(PipeClass::PipelineSet *pipelineSet, bool isSetTexture, int textureNumber)
{
	SetDrawBuffers(pipelineSet, isSetTexture, textureNumber);
	DrawCommand();
}

void Object3D::SetDrawBuffers(PipeClass::PipelineSet *pipelineSet, bool isSetTexture, int textureNumber)
{
	if (model == nullptr)
	{
		assert(0);
	}


	MyDirectX *myD = MyDirectX::Instance();

	myD->GetCommandList()->SetPipelineState(pipelineSet->pipelineState.Get());
	myD->GetCommandList()->SetGraphicsRootSignature(pipelineSet->rootSignature.Get());

	myD->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//デスクリプタヒープの配列

	ID3D12DescriptorHeap *descHeap = TextureMgr::Instance()->GetDescriptorHeap();
	ID3D12DescriptorHeap *ppHeaps[] = { descHeap };
	myD->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//定数バッファビュー
	myD->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	myD->GetCommandList()->SetGraphicsRootConstantBufferView(1, model->GetModel()->constBuffB1->GetGPUVirtualAddress());

	if (lightGroup != nullptr)
	{
		lightGroup->Draw(3);
	}
	if (isSetTexture)
	{
		if (!TextureMgr::Instance()->CheckHandle(textureNumber))
		{
			assert(0);
			return;
		}
		myD->GetCommandList()->SetGraphicsRootDescriptorTable(2,
			CD3DX12_GPU_DESCRIPTOR_HANDLE(
				descHeap->GetGPUDescriptorHandleForHeapStart(),
				textureNumber,
				myD->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			)
		);
	}
	else
	{
		if (!TextureMgr::Instance()->CheckHandle(model->GetModel()->textureHandle))
		{
			assert(0);
			return;
		}
		myD->GetCommandList()->SetGraphicsRootDescriptorTable(2,
			CD3DX12_GPU_DESCRIPTOR_HANDLE(
				descHeap->GetGPUDescriptorHandleForHeapStart(),
				model->GetModel()->textureHandle,
				myD->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			)
		);
	}

}

void Object3D::DrawCommand()
{
	if (model == nullptr)
	{
		assert(0);
	}
	MyDirectX *myD = MyDirectX::Instance();

#pragma region とりあえず引っ張り出した描画コマンド
	myD->GetCommandList()->IASetVertexBuffers(0, 1, &model->GetModel()->vbView);

	//インデックスバッファの設定
	myD->GetCommandList()->IASetIndexBuffer(&model->GetModel()->ibView);
	myD->GetCommandList()->DrawIndexedInstanced(static_cast<UINT>(model->GetModel()->indices.size()), 1, 0, 0, 0);
#pragma endregion
}

void Object3D::SetParent(Object3D *parent)
{
	if (parent == nullptr) return;
	this->parent = parent;
}

void Object3D::SetCamera(Camera *camera)
{
	if (camera == nullptr)
	{
		assert(0);
	}
	this->camera = camera;
}

void Object3D::SetLight(Light *light)
{
	this->light = light;
}

void Object3D::SetLightGroup(LightGroup *lightGroup)
{
	this->lightGroup = lightGroup;
}

void Object3D::SetCollider(BaseCollider *collider)
{
	collider->SetObject(this);
	this->collider = collider;
	//コリジョンマネージャーに登録
	CollisionManager::GetInstance()->AddCollider(collider);
	//コライダーを更新しておく
	collider->Update();
}

void Object3D::SetModel(Model *model)
{
	this->model = model;
}

void Object3D::SetPosition(XMFLOAT3 pos)
{
	position = pos;
}

void Object3D::SetColor(XMFLOAT4 color)
{
	this->color = color;
}

void Object3D::SetScale(XMFLOAT3 scale)
{
	this->scale = scale;
}

void Object3D::SetRotation(XMFLOAT3 rot)
{
	rotation = rot;

	quaternion = XMQuaternionRotationRollPitchYaw(rot.x, rot.y, rot.z);
	UpdateVector();
}

void Object3D::SetRotation(DirectX::XMVECTOR quaternion)
{
	this->quaternion = quaternion;
	UpdateVector();
}

void Object3D::AddRotation(DirectX::XMFLOAT3 rot)
{
	rotation.x += rot.x;
	rotation.y += rot.y;
	rotation.z += rot.z;
	XMVECTOR AddRot = XMQuaternionRotationRollPitchYaw(rot.x, rot.y, rot.z);
	//乗算処理すると回転を混ぜることができるよ
	quaternion = XMQuaternionMultiply(quaternion, AddRot);
	UpdateVector();
}

void Object3D::AddRotation(DirectX::XMVECTOR rot)
{
	quaternion = XMQuaternionMultiply(quaternion, rot);
	UpdateVector();
}

void Object3D::SetRotationVector(DirectX::XMVECTOR front, DirectX::XMVECTOR up)
{
	this->up = up;
	//正面ベクトルから回転行列を計算
	XMMATRIX matRot = FukuMath::GetMatRot(up, front);

	//行列をクオータニオンに変換
	quaternion = XMQuaternionRotationMatrix(matRot);

	//更新したクオータニオンを用いて各ベクトルを再計算
	UpdateVector();
}

const XMFLOAT4 &Object3D::GetColor()
{
	return color;
}

const XMFLOAT3 &Object3D::GetPosition()
{
	return position;
}

const DirectX::XMFLOAT3 Object3D::GetWorldPos()
{
	Vector3 worldPos = position;
	if (parent != nullptr)
	{
		worldPos *= Vector3(parent->GetScale());
		worldPos += parent->GetWorldPos();
	}
	return worldPos;
}

const XMFLOAT3 &Object3D::GetScale()
{
	return scale;
}

const XMFLOAT3 &Object3D::GetRotation()
{
	return rotation;
}

const DirectX::XMVECTOR &Object3D::GetQuaternion()
{
	return quaternion;
}

const DirectX::XMVECTOR &Object3D::GetRotQuaternion()
{
	return quaternion;
}

const DirectX::XMVECTOR Object3D::GerWorldQuaternion()
{
	XMVECTOR worldQ = quaternion;
	if (parent != nullptr)
	{
		XMVECTOR parentQ = GerWorldQuaternion();
		worldQ = XMQuaternionMultiply(worldQ, parentQ);
	}
	return worldQ;
}

const DirectX::XMVECTOR &Object3D::GetUpVec()
{
	return up;
}

const DirectX::XMVECTOR &Object3D::GetRightVec()
{
	return right;
}

const DirectX::XMVECTOR &Object3D::GetFrontVec()
{
	return front;
}

const Model *Object3D::GetModel()
{
	return model;
}

void Object3D::UpdateVector()
{
	front = XMVector3Rotate(ZVec, quaternion);
	up = XMVector3Rotate(YVec, quaternion);
	right = XMVector3Rotate(XVec, quaternion);
}

void Object3D::Separate()
{
	if (parent != nullptr)
	{
		//親オブジェクトのワールド行列を乗算する
		parent->GetMatWorld();
	}
}

void Object3D::ConnectObject(Object3D *parent)
{
	//位置情報をオブジェクト基準に変換する(逆行列をかけるような感じかな)

}

void DepthReset()
{
	MyDirectX *myDirectX = MyDirectX::Instance();
	myDirectX->GetCommandList()->ClearDepthStencilView(myDirectX->GetDsvH(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}
