#include "Object3D.h"
#include "TextureMgr.h"
#include "Model.h"
#include "../Collision/BaseCollider.h"
using namespace DirectX;


Object3D::~Object3D()
{
	if (collider)
	{
		delete collider;
	}
}

void Object3D::Init(Camera *camera, Light *light, Object3D *parent)
{

	this->parent = parent;

	HRESULT result = S_FALSE;
	MyDirectX *myD = MyDirectX::Instance();

	SetCamera(camera);
	SetLight(light);
	//���[���h�s���ݒ肷��
	matWorld = XMMatrixIdentity();

	matWorld = GetMatWorld();


	//�萔�o�b�t�@�̐���
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

		if (result)
		{
			assert(0);
		}
		isMakeConstBuffer = true;
	}
	
	ConstBufferData *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void **)&constMap);
	constMap->color = color;//�F�w��(RGBA)
	constMap->viewproj = camera->matView * camera->matProjection;
	constMap->world = matWorld;
	constMap->cameraPos = Vector3(camera->position) + Vector3(camera->eye);
	constBuff->Unmap(0, nullptr);

	//�N���X���̕�������擾
	name = typeid(*this).name();
}

const XMMATRIX Object3D::GetMatWorld()
{
	XMMATRIX matScale, matRot, matTrans, matTmp;


	//�e��A�t�B���ϊ����s��̌`�ɂ���
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation.z);
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//�e��ϊ��s�����Z���Ă䂭
	matTmp = XMMatrixIdentity();
	matTmp *= matScale;
	matTmp *= matRot;
	matTmp *= matTrans;

	//�����e�I�u�W�F�N�g������Ȃ�
	if (parent != nullptr)
	{
		//�e�I�u�W�F�N�g�̃��[���h�s�����Z����
		matTmp *= parent->GetMatWorld();
	}

	//�������ꂽ�s���Ԃ�
	return matTmp;
}

void Object3D::Update()
{
	//���[���h�s���ݒ肷��
	matWorld = GetMatWorld();

	ConstBufferData *constMap = nullptr;

	HRESULT result = constBuff->Map(0, nullptr, (void **)&constMap);
	constMap->color = color;//�F�w��(RGBA)
	constMap->viewproj = camera->matView * camera->matProjection;
	constMap->world = matWorld;
	constMap->cameraPos = Vector3(camera->position) + Vector3(camera->eye);
	constBuff->Unmap(0, nullptr);

	if (collider)
	{
		collider->Update();
	}
}


void Object3D::modelDraw(const ModelObject &model, PipeClass::PipelineSet pipelineSet, bool isSetTexture, int textureNumber)
{
	MyDirectX *myD = MyDirectX::Instance();


	myD->GetCommandList()->SetPipelineState(pipelineSet.pipelineState.Get());
	myD->GetCommandList()->SetGraphicsRootSignature(pipelineSet.rootSignature.Get());

	myD->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�f�X�N���v�^�q�[�v�̔z��

	ID3D12DescriptorHeap *descHeap = TextureMgr::Instance()->GetDescriptorHeap();
	ID3D12DescriptorHeap *ppHeaps[] = { descHeap };
	myD->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�萔�o�b�t�@�r���[
	myD->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	myD->GetCommandList()->SetGraphicsRootConstantBufferView(1, model.constBuffB1->GetGPUVirtualAddress());

	light->Draw(3);
	if(isSetTexture)
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
		if (!TextureMgr::Instance()->CheckHandle(model.textureHandle))
		{
			assert(0);
			return;
		}
		myD->GetCommandList()->SetGraphicsRootDescriptorTable(2,
			CD3DX12_GPU_DESCRIPTOR_HANDLE(
				descHeap->GetGPUDescriptorHandleForHeapStart(),
				model.textureHandle,
				myD->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			)
		);
	}

#pragma region �Ƃ肠������������o�����`��R�}���h
	myD->GetCommandList()->IASetVertexBuffers(0, 1, &model.vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	myD->GetCommandList()->IASetIndexBuffer(&model.ibView);
	myD->GetCommandList()->DrawIndexedInstanced(model.indices.size(), 1, 0, 0, 0);
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

void Object3D::SetCollider(BaseCollider *collider)
{
	collider->SetObject(this);
	this->collider = collider;
}

void DepthReset()
{
	MyDirectX *myDirectX = MyDirectX::Instance();
	myDirectX->GetCommandList()->ClearDepthStencilView(myDirectX->GetDsvH(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}
