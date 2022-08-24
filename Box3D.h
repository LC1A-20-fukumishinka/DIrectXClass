#pragma once
#include "MyDirectX.h"
#include <DirectXMath.h>
#include "Object3DCommon.h"
#include "../Camera.h"
#include "IGraphicsPipeline.h"
#include "../Light.h"
#include "../Collision/CollisionInfo.h"
#include "IGraphicsPipeline.h"
#include "Vector3.h"
/// <summary>
/// ���C�e�B���O�Ȃ��̋�`�I�u�W�F�N�g
/// </summary>
class BoxObj3D
{
private:
	struct ConstBufferData
	{
		DirectX::XMMATRIX mat;//���[���h�s��(��]�ƃX�P�[�����O�̂�)
		DirectX::XMMATRIX matViewprj;//�r���[�v���W�F�N�V�����s��
		DirectX::XMFLOAT4 color;//�F
	};
public:
	BoxObj3D();
	~BoxObj3D();

	void Init(const Vector3 pos, const Vector3 &front , const Vector3 &up);
	void Update();
	void Draw();
	void Finalize();

	const DirectX::XMMATRIX GetMatWorld();

	void SetCamera(Camera *camera);


	void SetRotation(DirectX::XMFLOAT3 rot);

	void SetRotation(DirectX::XMVECTOR quaternion);

	void AddRotation(DirectX::XMFLOAT3 rot);

	void AddRotation(DirectX::XMVECTOR rot);

	/// <summary>
	/// �p���𐳖ʂƏ�x�N�g������v�Z����
	/// </summary>
	/// <param name="front"></param>
	/// <param name="up"></param>
	void SetRotationVector(DirectX::XMVECTOR front, DirectX::XMVECTOR up);

private://�X�^�e�B�b�N�ϐ�
	static std::unique_ptr<PipeClass::PipelineSet> pipelineSet_;
private:
	void MakeVertexBuffer();
	void MakeIndexBuffer();
	void MakeConstBuffer();
private:
	// ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;

	void UpdateVector();

	//�F(RGBA)
	DirectX::XMFLOAT4 color = { 1.0f,1.0f ,1.0f ,1.0f };
	//�傫��
	Vector3 scale = { 10.0f ,10.0f ,10.0f };
	//�C�ӎ���]
	DirectX::XMVECTOR quaternion = {};
	//���W
	Vector3 position = { 0.0f,0.0f ,0.0f };
	DirectX::XMVECTOR front = { 0.0f,0.0f ,1.0f , 0.0f };
	DirectX::XMVECTOR up = { 0.0f,1.0f ,0.0f , 0.0f };
	DirectX::XMVECTOR right = { 1.0f, 0.0f, 0.0f, 0.0f };

	//�`��t���O
	bool isInvisible = false;

	//���[���h�s��
	DirectX::XMMATRIX matWorld;

	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;

	//�e�N�X�`���ԍ�
	UINT texNumber;

	Object3D *parent = nullptr;
	//��\��
	Camera *camera = nullptr;

	Light *light = nullptr;

	bool isMakeBuffers = false;


	ConstBufferData buffData;


};

