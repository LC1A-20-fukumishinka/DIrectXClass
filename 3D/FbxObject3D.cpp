#include "FbxObject3D.h"
#include "MyDirectX.h"
#include <d3dcompiler.h>
#include "FbxLoader.h"
#include "../BaseGraphicsPipeline.h"
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;
ComPtr<ID3D12RootSignature> FbxObject3D::rootsignature;
ComPtr<ID3D12PipelineState> FbxObject3D::pipelinestate;
ID3D12Device *FbxObject3D::device = nullptr;
Camera *FbxObject3D::camera = nullptr;
void FbxObject3D::FBXCreateGraphicsPipeline()
{
	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ //�e�����󂯂�{�[���ԍ�(4��)
			"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ //�{�[���̃X�L���E�F�C�g(4��)
			"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^
	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[3];
	// CBV�i���W�ϊ��s��p�j
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`���j
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	//CBV�X�L�j���O
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);


	PipelineDesc pipelineDesc;
	pipelineDesc.VSname = L"Resources/shaders/FBXVS.hlsl";
	pipelineDesc.PSname = L"Resources/shaders/FBXPS.hlsl";
	pipelineDesc.inputLayout = inputLayout;
	pipelineDesc.inputLayoutCount = _countof(inputLayout);
	pipelineDesc.rootparams = rootparams;
	pipelineDesc.rootparamsCount = _countof(rootparams);
	pipelineDesc.renderTargetCount = 2;
	//�֐��ō����
	std::unique_ptr<PipeClass::PipelineSet>tmpPipe = BaseGraphicsPipeline::CreatePipeLine(pipelineDesc);
	//���g��n����
	pipelinestate = tmpPipe->pipelineState;
	rootsignature = tmpPipe->rootSignature;

	//���L�������(��΂����Ƃ������@������͂��Ȃ񂾂��ǂȂ�)
	tmpPipe.release();
}
void FbxObject3D::SetDevice()
{
	FbxObject3D::device = MyDirectX::Instance()->GetDevice();
}
void FbxObject3D::Init()
{
	HRESULT result;
	if (device == nullptr)
	{
		assert(0);
	}
	//(���_�������)�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBufferTransform)
	);

	//(�X�L�j���O�������)�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin));

	//1�t���[�����̎��Ԃ�60FPS�Őݒ�
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataSkin *constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void **)&constMapSkin);
	for (int i = 0; i < MAX_BONES; i++)
	{
		constMapSkin->bones[i] = XMMatrixIdentity();
	}


	constBuffSkin->Unmap(0, nullptr);
}

void FbxObject3D::Update()
{
	XMMATRIX matScale, matRot, matTrans;

	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation.z);
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//���[���h�s��̍���
	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	const XMMATRIX &matViewProjection = camera->GetMatViewProj();

	const XMMATRIX &modelTransform = model->GetModeTransform();

	const XMFLOAT3 &cameraPos = camera->GetEye();

	HRESULT result;

	ConstBufferDataTransform *constMap = nullptr;
	result = constBufferTransform->Map(0, nullptr, (void **)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->viewproj = matViewProjection;
		constMap->world = modelTransform * matWorld;
		constMap->cameraPos = cameraPos;
		constBufferTransform->Unmap(0, nullptr);
	}

	//�{�[���z��
	std::vector<FbxModel::Bone> &bones = model->GetBones();

	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataSkin *constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void **)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		//���̎p���s��
		XMMATRIX matCurrentPose;
		//���̎p���s����擾
		FbxAMatrix fbxCurrentPose =
			bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		//XMMATRIX�ɕϊ�
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//�������ăX�L�j���O�s���
		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);

	if (isPlay)
	{
		//1�t���[���i�߂�
		currentTime += frameTime;
		//�Ō�܂ōĐ�������擪�ɖ߂�
		if (currentTime > endTime)
		{
			currentTime = startTime;
		}
	}
}

void FbxObject3D::Draw()
{
	ID3D12GraphicsCommandList *cmdList = MyDirectX::Instance()->GetCommandList();

	if (model == nullptr)
	{
		return;
	}
	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�i�A�t�B���ϊ��n�́j�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBufferTransform->GetGPUVirtualAddress());

	//�i�X�L�j���O�n�́j�萔�o�b�t�@�r���[�̃Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());
	//���f���̕`��
	model->Draw();
}

void FbxObject3D::PlayAnimation()
{
	FbxScene *fbxScene = model->GetFbxScene();
	//0�Ԃ̃A�j���[�V�����擾
	FbxAnimStack *animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);
	//�A�j���[�V�����̖��O�擾
	const char *animstackname = animstack->GetName();
	//�A�j���[�V�����̎��ԏ��
	FbxTakeInfo *takeinfo = fbxScene->GetTakeInfo(animstackname);

	//�J�n���Ԏ擾
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	//�I�����Ԏ擾
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	//�J�n���Ԃɍ��킹��
	currentTime = startTime;
	//�Đ�����Ԃɂ���
	isPlay = true;
}
