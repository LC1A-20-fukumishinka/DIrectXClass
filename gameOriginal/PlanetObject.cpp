#include "PlanetObject.h"
#include "TextureMgr.h"
#include "../Collision/BaseCollider.h"
#include "../Collision/CollisionMgr.h"
#include "Model.h"
using namespace DirectX;
void PlanetObject::Init()
{
	HRESULT result = S_FALSE;
	MyDirectX *myD = MyDirectX::Instance();
	//���[���h�s���ݒ肷��

	quaternion = XMQuaternionIdentity();
	matWorld = XMMatrixIdentity();

	matWorld = GetMatWorld();


	//�萔�o�b�t�@�̐���
	if (!Object3D::isMakeConstBuffer)
	{
		HRESULT result = S_FALSE;
		MyDirectX *myD = MyDirectX::Instance();

		result = myD->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(PlanetConstBufferData) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff)
		);

		constBuff->SetName(L"PlanetConstBuff3D");
		if (result)
		{
			assert(0);
		}


		result = myD->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(PlanetAnimationData) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&planetAnimationDataBuff)
		);

		planetAnimationDataBuff->SetName(L"PlanetAnimationDataBuff");
		if (result)
		{
			assert(0);
		}

		SendPlanetAnimationData(Vector3(1, 0, 0), 0.0f);

		isMakeConstBuffer = true;
	}

	//�N���X���̕�������擾
	name = typeid(*this).name();
}

void PlanetObject::Update()
{
	//���[���h�s���ݒ肷��
	matWorld = GetMatWorld();

	if (camera == nullptr || shadowCamera == nullptr)
	{
		assert(0);
	}
	PlanetConstBufferData *constMap = nullptr;

	HRESULT result = constBuff->Map(0, nullptr, (void **)&constMap);
	constMap->color = color;//�F�w��(RGBA)
	constMap->viewproj = camera->GetMatViewProj();
	constMap->world = matWorld;
	constMap->cameraPos = Vector3(camera->GetEye());
	constMap->shadowViewproj = shadowCamera->GetMatViewProj();
	constBuff->Unmap(0, nullptr);

	if (collider)
	{
		collider->Update();
	}
}

void PlanetObject::modelDraw(PipeClass::PipelineSet *pipelineSet, bool isSetTexture, int textureNumber)
{
	if (model == nullptr)
	{
		assert(0);
	}

	if (lightGroup == nullptr)
	{
		assert(0);
	}
	MyDirectX *myD = MyDirectX::Instance();

	myD->GetCommandList()->SetPipelineState(pipelineSet->pipelineState.Get());
	myD->GetCommandList()->SetGraphicsRootSignature(pipelineSet->rootSignature.Get());

	myD->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�f�X�N���v�^�q�[�v�̔z��

	ID3D12DescriptorHeap *descHeap = TextureMgr::Instance()->GetDescriptorHeap();
	ID3D12DescriptorHeap *ppHeaps[] = { descHeap };
	myD->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�萔�o�b�t�@�r���[
	myD->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	myD->GetCommandList()->SetGraphicsRootConstantBufferView(1, model->GetModel()->constBuffB1->GetGPUVirtualAddress());

	myD->GetCommandList()->SetGraphicsRootConstantBufferView(3, planetAnimationDataBuff->GetGPUVirtualAddress());


	lightGroup->Draw(2);

	//�e�N�X�`���̐ݒ�
	int modelTexture = -1;
	if (isSetTexture)
	{
		modelTexture = textureNumber;
	}
	else
	{
		modelTexture = model->GetModel()->textureHandle;
	}

	if (!TextureMgr::Instance()->CheckHandle(modelTexture) || !TextureMgr::Instance()->CheckHandle(shadowTexture))
	{
		assert(0);
		return;
	}
	myD->GetCommandList()->SetGraphicsRootDescriptorTable(4,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeap->GetGPUDescriptorHandleForHeapStart(),
			modelTexture,
			myD->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);
	myD->GetCommandList()->SetGraphicsRootDescriptorTable(5,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeap->GetGPUDescriptorHandleForHeapStart(),
			shadowTexture,
			myD->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);

#pragma region �Ƃ肠������������o�����`��R�}���h
	myD->GetCommandList()->IASetVertexBuffers(0, 1, &model->GetModel()->vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	myD->GetCommandList()->IASetIndexBuffer(&model->GetModel()->ibView);
	myD->GetCommandList()->DrawIndexedInstanced(static_cast<UINT>(model->GetModel()->indices.size()), 1, 0, 0, 0);
#pragma endregion
}

void PlanetObject::SendPlanetAnimationData(const Vector3 &PlanetCenterPos, float Rate)
{
	PlanetAnimationData *constMap = nullptr;

	HRESULT result = planetAnimationDataBuff->Map(0, nullptr, (void **)&constMap);
	constMap->planetToPlayerAngle = PlanetCenterPos;
	constMap->rate = Rate;
	constMap->planetPos = position;
	planetAnimationDataBuff->Unmap(0, nullptr);
}

void PlanetObject::SetShadowCamera(Camera *cam)
{
	shadowCamera = cam;
}

void PlanetObject::SetShadowTextureNum(int shadowTextureNum)
{
	shadowTexture = shadowTextureNum;
}
