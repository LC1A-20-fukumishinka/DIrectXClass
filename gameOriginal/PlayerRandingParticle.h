#pragma once
#include "particleManager.h"
class PlayerRandingParticle : public ParticleManager
{
public:
	//�R���X�g���N�^
	PlayerRandingParticle();
	//�f�X�g���N�^
	~PlayerRandingParticle();
	//�X�V����
	void Update() override;

	/// <summary>
	/// ���n���̃p�[�e�B�N����������
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="up">�L�����N�^�[�̏����</param>
	void AddRandingParticle(const Vector3& pos, const Vector3& up, const Vector3 &color);
};

