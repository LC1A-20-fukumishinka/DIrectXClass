#pragma once
#include "../Base/MyDirectX.h"
#include <DirectXMath.h>
#include "Object3DCommon.h"
#include "../Camera.h"
#include "GraphicsPipeline.h"
class Object3D
{
public:

	struct ConstBufferData
	{
		DirectX::XMFLOAT4 color;	//�F(RGBA)
		DirectX::XMMATRIX mat;	//3D�ϊ��s��
	};
	enum ObjectName
	{
		Corn,
		Box,
		Plane
	};
	Object3D();

	void Init(const Object3DCommon &object3DCommon,const Camera &camera, UINT texNumber, Object3D *parent = nullptr);

	//void Object3DTranceferVertexBuffer(const Object3DCommon &objCommon);

	//void Objecte3DDraw();

	//void Object3DUpdate();

	//void DrawObject3D(ID3D12DescriptorHeap *descHeap, D3D12_VERTEX_BUFFER_VIEW &vbView, D3D12_INDEX_BUFFER_VIEW &iBView, UINT numIndices);

	DirectX::XMMATRIX GetMatWorld();

	void Update(const Camera &camera);

	void SetConstBuffer(const Camera &camera);

	void Draw(const Object3DCommon &object3DCommon, PipeClass::PipelineSet pipelineSet, int textureNumber);

	void SetParent(Object3D *parent);
	//�F(RGBA)
	DirectX::XMFLOAT4 color;
	//�傫��
	DirectX::XMFLOAT3 scale;
	//��]
	DirectX::XMFLOAT3 rotation;
	//���W
	DirectX::XMFLOAT3 position;
	//�`��t���O
	bool isInvisible;
	//�ǂ�Ȍ`��
	ObjectName type;

	//���[���h�s��
	DirectX::XMMATRIX matWorld;
	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;

private:

	//�e�N�X�`���ԍ�
	UINT texNumber;

	Object3D *parent;
	//��\��




	//�v��Ȃ�

	//struct Vertex
	//{
	//	DirectX::XMFLOAT3 pos;	//xyz���W
	//	DirectX::XMFLOAT3 normal;
	//	DirectX::XMFLOAT2 uv;	//uv���W
	//};
	//Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;				//���_�o�b�t�@
	//Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	//D3D12_VERTEX_BUFFER_VIEW vBView{};			//���_�o�b�t�@�r���[
	//D3D12_INDEX_BUFFER_VIEW iBView{};

	//void CornInit(const Object3DCommon &object3DCommon);
	//void BoxInit(const Object3DCommon &object3DCommon);



	//void CornTransferIndexBufferVertexBuffer(const Object3DCommon &object3DCommon);
};

//�I�u�W�F�N�g3D�̃R�}���h(Object3DDraw�̑O�ɕK�v)
//void Object3DCommonBeginDraw(const Object3DCommon &Object3DCommon, const SpriteCommon &spriteCommon);
