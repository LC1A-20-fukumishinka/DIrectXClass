#pragma once
#include "Object3D.h"
#include <memory>
#include "Vector3.h"

class Model;
class Planet;

/// <summary>
/// ���ʏ�ɔz�u�����I�u�W�F�N�g�N���X
/// </summary>
class ObjectOnSphere
{
public://��b�֐�
	ObjectOnSphere();
	~ObjectOnSphere();

	virtual void Init(std::weak_ptr<Planet> basePlanet, Vector3 angle, float scale);
	virtual void Update();
	virtual void Draw();
	virtual void Finalize();

protected://�����o�֐�

	//Model�����蓖�Ă�p
	void SetModel(Model *model);


	Vector3 GetWorldPos();
	float GetWorldScale();
	DirectX::XMVECTOR GetRotationQ();

	bool GetIsPlanetSpawn();
	///// <summary>
	///// �����œ��ꂽ������������
	///// </summary>
	///// <param name="MoveVec">�������x�N�g��</param>
	//void Move(Vector3 MoveVec);
protected://staic�ϐ�
	//�Q�[���̃��C���J����
	static Camera *sMainCamera;
	//�Q�[���̃��C�g
	static LightGroup *sLights;

public://static�֐�

	/// <summary>
	/// �Q�[�����̃��C���J�����̃Z�b�^
	/// </summary>
	/// <param name="camera">���铖�Ă�J�����̃|�C���^(�ŏI�I�ɂ̓X�}�[�g�|�C���^������)</param>
	static void SetCamera(Camera *camera);

	/// <summary>
	/// ���X����
	/// </summary>
	/// <param name="lights">���蓖�Ă����</param>
	static void SetLights(LightGroup *lights);
private://�����o�֐�
	void RotationUpdate();
private://�����o�ϐ�

	Object3D shadowObject_;
	Object3D object_;

	std::weak_ptr<Planet> basePlanet_;

	Vector3 upVec_ = { 0, 1, 0 };
	Vector3 frontVec_ = { 0, 0, 1 };
	float worldScale_ = 1.0f;
};



