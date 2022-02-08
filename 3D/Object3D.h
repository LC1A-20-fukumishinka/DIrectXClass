#pragma once
#include "MyDirectX.h"
#include <DirectXMath.h>
#include "Object3DCommon.h"
#include "../Camera.h"
#include "IGraphicsPipeline.h"
#include "../Light.h"
#include "../Collision/CollisionInfo.h"
struct ModelObject;

class BaseCollider;
class Object3D
{
public:

	struct ConstBufferData
	{
		DirectX::XMFLOAT4 color;	//�F(RGBA)
		//DirectX::XMMATRIX mat;	//3D�ϊ��s��
		DirectX::XMMATRIX viewproj;
		DirectX::XMMATRIX world;
		DirectX::XMFLOAT3 cameraPos;
	};
	enum ObjectName
	{
		Corn,
		Box,
		Plane
	};
	Object3D() = default;

	virtual ~Object3D();
	virtual void Init(Camera *camera, Light *light, Object3D *parent = nullptr);

	const DirectX::XMMATRIX GetMatWorld();

	virtual void Update();


	/// <summary>
	/// ���f���̕`��R�}���h
	/// </summary>
	/// <param name="isSetTexture">�ʂ̃e�N�X�`�����g�p����</param>
	/// <param name="textureNumber">�g�p����e�N�X�`���̃n���h��</param>
	virtual void modelDraw(PipeClass::PipelineSet pipelineSet, bool isSetTexture = false, int textureNumber = -1);

	virtual void OnCollision(const CollisionInfo &info) {};

public://�Z�b�^

	void SetParent(Object3D *parent);

	void SetCamera(Camera *camera);

	void SetLight(Light *light);

	void SetCollider(BaseCollider *collider);

	void SetModel(ModelObject *model);

	void SetPosition(DirectX::XMFLOAT3 pos);
	void SetColor(DirectX::XMFLOAT4 color);
	void SetScale(DirectX::XMFLOAT3 scale);
	void SetRotation(DirectX::XMFLOAT3 rot);
public:
	const DirectX::XMFLOAT4 &GetColor();
	const DirectX::XMFLOAT3 &GetPosition();
	const DirectX::XMFLOAT3 &GetScale();
	const DirectX::XMFLOAT3 &GetRotation();
protected:
	//�N���X��(�f�o�b�O�p)
	const char *name = nullptr;

	//�R���C�_�[
	BaseCollider *collider = nullptr;

	//�F(RGBA)
	DirectX::XMFLOAT4 color = { 1.0f,1.0f ,1.0f ,1.0f };
	//�傫��
	DirectX::XMFLOAT3 scale = { 1.0f ,1.0f ,1.0f };
	//��]
	DirectX::XMFLOAT3 rotation = { 0.0f,0.0f ,0.0f };
	//���W
	DirectX::XMFLOAT3 position = { 0.0f,0.0f ,0.0f };
	//�`��t���O
	bool isInvisible = false;

	//���[���h�s��
	DirectX::XMMATRIX matWorld;

private:
	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;

	//�e�N�X�`���ԍ�
	UINT texNumber;

	Object3D *parent = nullptr;
	//��\��
	Camera *camera = nullptr;

	Light *light = nullptr;

	bool isMakeConstBuffer = false;

	ModelObject *model = nullptr;
};

//�[�x�l���Z�b�g
void DepthReset();
