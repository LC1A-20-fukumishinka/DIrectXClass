#pragma once
#include "Object3D.h"
#include "particleManager.h"

class Player : public Object3D
{
public:
	/// <summary>
	/// 3D�I�u�W�F�N�g�̐���
	/// </summary>
	/// <returns></returns>
	static Player *Create(Camera *camera, Light *light, Object3D *parent = nullptr);
	~Player();
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="light"></param>
	/// <param name="parent"></param>
	void Init()override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update()override;

	/// <summary>
	/// �Փˎ��̃R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info"></param>
	void OnCollision(const CollisionInfo &info) override;
	void modelDraw(PipeClass::PipelineSet *pipelineSet, bool isSetTexture = false, int textureNumber = -1)override; 

	void SetCamera(Camera *camera)override;
private:
	int spriteTex;
	ParticleManager *hitPart;
};