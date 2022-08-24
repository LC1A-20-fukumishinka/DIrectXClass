#include "Box3D.h"
#include "TextureMgr.h"
#include "FukuMath.h"
using namespace FukuMath;
using namespace DirectX;

std::unique_ptr<PipeClass::PipelineSet> BoxObj3D::pipelineSet_;

BoxObj3D::BoxObj3D()
{
	buffData.mat = XMMatrixIdentity();
	buffData.matViewprj = XMMatrixIdentity();
	buffData.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	if (!pipelineSet_)
	{
		D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ // �Ƃ肠���������Ă邾���̍��W(1���_�����Ȃ̂ŕK�v�Ȃ���΍ŏI�I�ɂȂ���)
				"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
		};

		CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
		descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);//t0���W�X�^

		//���[�g�p�����^
		CD3DX12_ROOT_PARAMETER rootparams[2];
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);			//���W�ϊ� + �`��F
		rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);	//�e�N�X�`���[�pSRV

		PipelineDesc PipelineDesc;
		PipelineDesc.VSname = L"Resources/shaders/BoxShaderVS.hlsl";
		PipelineDesc.PSname = L"Resources/shaders/BoxShaderPS.hlsl";
		PipelineDesc.inputLayout = inputLayout;
		PipelineDesc.inputLayoutCount = _countof(inputLayout);
		PipelineDesc.rootparams = rootparams;
		PipelineDesc.rootparamsCount = _countof(rootparams);
		pipelineSet_ = BaseGraphicsPipeline::CreatePipeLine(PipelineDesc, L"Resources/shaders/BoxShaderGS.hlsl");
	}
}

BoxObj3D::~BoxObj3D()
{
	vertBuff.Reset();
	indexBuff.Reset();
}

void BoxObj3D::Init(const Vector3 pos, const Vector3 &front, const Vector3 &up)
{
	HRESULT result = S_FALSE;

	//���[���h�s���ݒ肷��
	quaternion = XMQuaternionIdentity();
	matWorld = XMMatrixIdentity();

	position = pos;
	this->front = XMLoadFloat3(&front);
	this->up = XMLoadFloat3(&up);
	SetRotationVector(this->front, this->up);
	matWorld = GetMatWorld();

	//�萔�o�b�t�@�̐���
	if (!isMakeBuffers)
	{
		MakeVertexBuffer();
		MakeIndexBuffer();
		MakeConstBuffer();
	}

}

void BoxObj3D::Update()
{
	//���[���h�s���ݒ肷��
	matWorld = GetMatWorld();

	if (camera == nullptr)
	{
		assert(0);
	}

	buffData.color = color;
	buffData.matViewprj = camera->GetMatViewProj();
	buffData.mat = matWorld;

	void *constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, &constMap);
	std::memcpy(constMap, &buffData, sizeof(buffData));
	constBuff->Unmap(0, nullptr);
}

void BoxObj3D::Draw()
{
	MyDirectX *myD = MyDirectX::Instance();

	myD->GetCommandList()->SetPipelineState(pipelineSet_->pipelineState.Get());
	myD->GetCommandList()->SetGraphicsRootSignature(pipelineSet_->rootSignature.Get());

	myD->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//�f�X�N���v�^�q�[�v�̔z��

	ID3D12DescriptorHeap *descHeap = TextureMgr::Instance()->GetDescriptorHeap();
	ID3D12DescriptorHeap *ppHeaps[] = { descHeap };
	myD->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�萔�o�b�t�@�r���[
	myD->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	if (!TextureMgr::Instance()->CheckHandle(texNumber))
	{
		assert(0);
		return;
	}
	myD->GetCommandList()->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeap->GetGPUDescriptorHandleForHeapStart(),
			texNumber,
			myD->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);

#pragma region �Ƃ肠������������o�����`��R�}���h
	myD->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	myD->GetCommandList()->IASetIndexBuffer(&ibView);
	myD->GetCommandList()->DrawIndexedInstanced(static_cast<UINT>(1), 1, 0, 0, 0);
	//myD->GetCommandList()->DrawInstanced(1, 1, 0, 0);

#pragma endregion

}

void BoxObj3D::Finalize()
{
}

const XMMATRIX BoxObj3D::GetMatWorld()
{
	XMMATRIX matScale, matRot, matTrans, matTmp;


	//�e��A�t�B���ϊ����s��̌`�ɂ���
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot = XMMatrixRotationQuaternion(quaternion);
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//�I�u�W�F�N�g�̎��̃x�N�g�����X�V

	//���ʃx�N�g�����Čv�Z
	UpdateVector();

	//�e��ϊ��s�����Z���Ă䂭
	matTmp = XMMatrixIdentity();
	matTmp *= matScale;
	matTmp *= matRot;
	matTmp *= matTrans;

	//�������ꂽ�s���Ԃ�
	return matTmp;
}

void BoxObj3D::SetCamera(Camera *camera)
{
	this->camera = camera;
}

void BoxObj3D::MakeVertexBuffer()
{
	//1���_���Ƃ肠�����p�ӂ���
	UINT sizeVB = static_cast<UINT>(sizeof(DirectX::XMFLOAT3));

	ID3D12Device *device = MyDirectX::Instance()->GetDevice();
	// ���_�o�b�t�@����
	HRESULT result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		//&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	vertBuff->SetName(L"BoxVert");

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	XMFLOAT3 *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		*vertMap = XMFLOAT3();
		vertBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(XMFLOAT3);
}

void BoxObj3D::MakeIndexBuffer()
{
	ID3D12Device *device = MyDirectX::Instance()->GetDevice();

	//�C���f�b�N�X���
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short));
	// �C���f�b�N�X�o�b�t�@����
	HRESULT result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		//&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	indexBuff->SetName(L"BoxIndex");
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short *indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void **)&indexMap);
	if (SUCCEEDED(result)) {
		*indexMap = 0;
		indexBuff->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

void BoxObj3D::MakeConstBuffer()
{
	HRESULT result = S_FALSE;
	ID3D12Device *device = MyDirectX::Instance()->GetDevice();

	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);

	constBuff->SetName(L"BoxConstBuff3D");
	if (result)
	{
		assert(0);
	}
	isMakeBuffers = true;
}

void BoxObj3D::UpdateVector()
{
	front = XMVector3Rotate(ZVec, quaternion);
	up = XMVector3Rotate(YVec, quaternion);
	right = XMVector3Rotate(XVec, quaternion);
}

void BoxObj3D::SetRotation(XMFLOAT3 rot)
{
	quaternion = XMQuaternionRotationRollPitchYaw(rot.x, rot.y, rot.z);
	UpdateVector();
}

void BoxObj3D::SetRotation(DirectX::XMVECTOR quaternion)
{
	this->quaternion = quaternion;
	UpdateVector();
}

void BoxObj3D::AddRotation(DirectX::XMFLOAT3 rot)
{
	XMVECTOR AddRot = XMQuaternionRotationRollPitchYaw(rot.x, rot.y, rot.z);
	//��Z��������Ɖ�]�������邱�Ƃ��ł����
	quaternion = XMQuaternionMultiply(quaternion, AddRot);
	UpdateVector();
}

void BoxObj3D::AddRotation(DirectX::XMVECTOR rot)
{
	quaternion = XMQuaternionMultiply(quaternion, rot);
	UpdateVector();
}

void BoxObj3D::SetRotationVector(DirectX::XMVECTOR front, DirectX::XMVECTOR up)
{
	this->up = up;
	//���ʃx�N�g�������]�s����v�Z
	XMMATRIX matRot = FukuMath::GetMatRot(front, up);

	//�s����N�I�[�^�j�I���ɕϊ�
	quaternion = XMQuaternionRotationMatrix(matRot);

	//�X�V�����N�I�[�^�j�I����p���Ċe�x�N�g�����Čv�Z
	UpdateVector();
}