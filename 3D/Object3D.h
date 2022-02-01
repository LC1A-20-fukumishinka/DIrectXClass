#pragma once
#include "../Base/MyDirectX.h"
#include <DirectXMath.h>
#include "Object3DCommon.h"
#include "../Camera.h"
#include "IGraphicsPipeline.h"
#include "../Light.h"
struct ModelObject;

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
	Object3D();

	void Init(Camera *camera, Light *light, Object3D *parent = nullptr);

	DirectX::XMMATRIX GetMatWorld();

	void Update();

	void Draw(const Object3DCommon &object3DCommon, PipeClass::PipelineSet pipelineSet, int textureNumber);

	/// <summary>
	/// ���f���̕`��R�}���h
	/// </summary>
	/// <param name="isSetTexture">�ʂ̃e�N�X�`�����g�p����</param>
	/// <param name="textureNumber">�g�p����e�N�X�`���̃n���h��</param>
	void modelDraw(const ModelObject &model, PipeClass::PipelineSet pipelineSet, bool isSetTexture = false, int textureNumber = -1);

	void SetParent(Object3D *parent);

	void SetCamera(Camera *camera);

	void SetLight(Light *light);
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

	Object3D *parent = nullptr;
	//��\��
	Camera *camera = nullptr;

	Light *light = nullptr;
};

//�[�x�l���Z�b�g
void DepthReset();
