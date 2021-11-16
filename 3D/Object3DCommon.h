#pragma once
#include "../Base/MyDirectX.h"
#include<DirectXMath.h>

class Object3DCommon
{
public:
	struct PipelineSet
	{
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;	//���[�g�V�O�l�`��
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;	//�p�C�v���C���X�e�[�g
	};
	enum Sprites
	{
		Object3DSRVCount = 512
	};
	enum
	{
		CornNumIndices = 18,
		BoxNumIndices = 36,
		PlaneNumIndices = 6
	};
	Object3DCommon();
	//�ˉe�s��
	DirectX::XMMATRIX matProjection{};

	//DirectX::XMMATRIX matView{};

	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> descHeap;
	//�e�N�X�`�����\�[�X(�e�N�X�`���o�b�t�@)�̔z��
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[Object3DSRVCount];
	void Init();

	void SpriteLoadTexture(UINT texnumber, const wchar_t *filename);

	Microsoft::WRL::ComPtr<ID3D12Resource> cornVertBuff;				//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> cornIndexBuff;
	D3D12_VERTEX_BUFFER_VIEW cornVBView{};			//���_�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW cornIBView{};

	Microsoft::WRL::ComPtr<ID3D12Resource> boxVertBuff;				//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> boxIndexBuff;
	D3D12_VERTEX_BUFFER_VIEW boxVBView{};			//���_�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW boxIBView{};

	Microsoft::WRL::ComPtr<ID3D12Resource> planeVertBuff;				//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> planeIndexBuff;
	D3D12_VERTEX_BUFFER_VIEW planeVBView{};			//���_�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW planeIBView{};
private:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;	//xyz���W
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;	//uv���W
	};



	void CornInit();
	void BoxInit();
	void PlaneInit();


	void CornTransferIndexBufferVertexBuffer();
	void BoxTransferIndexBufferVertexBuffer();
	void PlaneTransferIndexBufferVertexBuffer();
};