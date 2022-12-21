#pragma once
#include "FieldParticle.h"
#include "PlanetParticle.h"
#include "GateParticle.h"
#include "gameConstData.h"
class Camera;

/// <summary>
/// �Q�[�����̃p�[�e�B�N�����܂Ƃ߂ĊǗ�����N���X
/// </summary>
class GranetParticleManager
{
public:
	/// <summary>
	/// �p�[�e�B�N���𐶐�����̂ɕK�v�ȃf�[�^
	/// </summary>
	struct MakeParticleDatas
	{
		//�J�����̍��W
		Vector3 cameraPos;
		//�J�����̎p��
		DirectX::XMVECTOR cameraRot;
		//�v���C���[�̎��d�̓f�[�^
		GameDatas::GravityData playerGravity;
		//�v���C���[�̍��W
		Vector3 playerPos;
		//�v���C���[��
		Vector3 playerMoveVec;
	};
public:
	//�R���X�g���N�^
	GranetParticleManager();
	//�f�X�g���N�^
	~GranetParticleManager();
	//������
	void Init(Camera* camera);
	//�X�V����
	void Update();
	//�`��
	void Draw();

	//�p�[�e�B�N���̐����ɕK�v�ȏ��
	void SetMakeParticleDatas(const MakeParticleDatas &data);
	//PlanetParticle�̃I�u�W�F�N�g�̃|�C���^
	PlanetParticle* GetPlanetParticles();
	//GateParticle�̃I�u�W�F�N�g�̃|�C���^
	GateParticle* GetGateParticles();
private:
	//�S�̂ɏ�ɏo�Ă���p�[�e�B�N��
	FieldParticle fieldParticles_;
	//�f���̐F���ς�����ۂɏo��p�[�e�B�N��
	PlanetParticle planetParticles_;
	//�����O�ʉߎ��ɐ��������p�[�e�B�N��
	GateParticle gateParticles_;

	//�����ɕK�v�ȃf�[�^
	MakeParticleDatas makeData;
};

