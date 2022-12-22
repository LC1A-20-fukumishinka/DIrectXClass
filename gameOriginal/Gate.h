#pragma once
#include "Object3D.h"
#include "Vector3.h"
#include "ModelManager.h"
#include <memory>
#include "gameConstData.h"
#include "../Collision/CollisionPrimitive.h"
class Gate
{
public:
	//�R���X�g���N�^
	Gate();
	//�f�X�g���N�^
	~Gate();

	//��������������
	void Init(const Vector3 &pos, const Vector3 &angle, int index, bool isDraw);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw();
	//�I������
	void Finalize();
	//���Z�b�g����(�Q�[�����Z�b�g���Ɏg�p����)
	void Reset();
	//��������(�����O�𕜊����������Ƃ��Ɏg��)
	void Reborn();
	//�Փˏ���
	bool Collision(const Sphere &player);
	//�ꏊ�̃Q�b�^
	const DirectX::XMFLOAT3 &GetPos();
	//�����Ă�����̃Q�b�^
	Vector3 GetAngle();
public:
	static void SetModel(Model *model);
	static void SetCamera(Camera *camera);
	static void SetLightGroup(LightGroup *lights);
private:

	// �@�\���Ă����Ԃ�
	bool isAlive_ = false;
	//�`�悵�Ă��邩
	bool isDraw_ = true;

	//���݂̍��W
	Vector3 pos_;

	//�����O�̌���
	Vector3 angle_;

	//���a
	float scale_ = 30.0f;


	//�����̃X�e�[�W�ԍ�
	int index_ = -1;

	//�`��p�I�u�W�F�N�g
	Object3D object_;

	//���f��
	static Model *sModel;
	//�J����
	static Camera *sCamera;
	//���C�g
	static LightGroup *sLights;

};

