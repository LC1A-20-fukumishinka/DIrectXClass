#pragma once
#include "Object3D.h"
#include "Vector3.h"
#include "Model.h"
#include <memory>
#include "gameConstData.h"
#include "../Collision/CollisionPrimitive.h"
class Planet;

enum class PlayerStatus
{
	STAND,
	JUMP,
};
class Player
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;
public://��{�֐�

	/// <summary>
	/// �������֐�
	/// </summary>
	/// <param name="model"></param>
	void Init(std::shared_ptr<Planet> planet);
	void Update();
	void Finalize();
	void Draw();
	void ShadowDraw();
private:
	/// <summary>
	/// �X�e�B�b�N�ɂ��ړ��֐�
	/// </summary>
	/// <param name="isSetAngle">���͂ɂ������̕ω������邩</param>
	void Move(bool isSetAngle);

	/// <summary>
	/// ���W�̈ړ�
	/// </summary>
	/// <param name="move">�X�e�B�b�N���͂̈ړ���</param>
	void PosUpdate(const Vector3 &move);

	/// <summary>
	/// �p���̕ό`
	/// </summary>
	/// <param name="move">�X�e�B�b�N���͂̈ړ��ړ�</param>
	void PostureUpdate(const Vector3 &move);
	void FloorMove(bool isSetAngle);

	void JumpMove(bool isSetAngle);
	/// <summary>
	/// player���E�X�e�B�b�N�ŉ�]�ł���悤�ɂ���
	/// </summary>
	void PlayerRotation();

	void PostureReset();
private://��Ԗ��̋����Ǘ�

	//�ʏ펞����
	void NormalUpdate();

	void LockOnUpdate();
public://�Q�b�^�Z�b�^
	void SetPos(const DirectX::XMFLOAT3 &pos);

	void SetRotation(const DirectX::XMFLOAT3 &rot);
	void SetModel(Model *model);
	void SetCamera(Camera *cam);
	void SetShadowCamera(Camera *cam);
	void SetLight(LightGroup *lights);
	const XMFLOAT3 &GetPos();
	const XMFLOAT3 GetAngle();
	const XMFLOAT3 GetUpVec();
	float GetBasePlanetScale();
	const std::weak_ptr<Planet> &GetBasePlanet();
	GameDatas::GravityData GetGravityData();
	bool GetIsJump();
	bool GetIsOneWayGravity();
public://�f������̏���
	/// <summary>
	/// �͂ޘf���̃Z�b�g���s��
	/// </summary>
	/// <param name="planet">�͂ޘf���̃|�C���^</param>
	void SetGrabPlanet(std::shared_ptr<Planet> planet);

	void SetBasePlanet(std::shared_ptr<Planet> planet);

	/// <summary>
	/// �͂�ł���f���𗣂�
	/// </summary>
	void ReleasePlanet();

	//�͂�ł���ۂ̍X�V�����Ђ���Ƃ�����v��Ȃ�����
	void GrabUpdate();

	void BlockCollision(const std::vector<Triangle> &boxPlanes);

	void AddGravity(Vector3 gravity);

	void shakeUpdate(float shakePower);

	void LoadModel();
private:
	Vector3 pos_;
	Vector3 rotation;
	Vector3 scale;
	Object3D drawObject;
	Object3D shadowObject;
	Camera *cam;
	Camera *shadowCamera;
	std::weak_ptr<Planet> grabPlanet;

	std::weak_ptr<Planet> basePlanet;

	PlayerStatus status;

	DirectX::XMVECTOR playerAngle;

	Vector3 warkVec_;

	Vector3 gravityVec_;

	Vector3 moveVec_;
	//�f���Ǝ����̋���
	float baseLength = 0.0f;

	//�f������̍�����(�ʒu)
	float localHeight = 0.0f;
	//�W�����v���̑��x(���x)
	float jumpSpeed = 0.0f;

	//�W�����v��
	const float jumpPower = 0.5f;

	bool oldLockonInput = false;

	GameDatas::GravityData gravity_;
	//�P�������d��
	Vector3 oneWayGravityAngle_;

	//���E����̏d��
	Vector3 worldGravity_;

	//�v���C���[�ɍŏI�I�ɂ�����d��

	int shakeTimer_ = 5;
	int shakeTimerMax_ = 15;

	std::unique_ptr<Model> playerModel_;
	std::unique_ptr<Model> ArrowModel_;
};

