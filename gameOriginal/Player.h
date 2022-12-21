#pragma once
#include "Object3D.h"
#include "Vector3.h"
#include "Model.h"
#include <memory>
#include "gameConstData.h"
#include "../Collision/CollisionPrimitive.h"
class Planet;

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

	void ImguiDraw();
public://�ŗL�֐�
	void Boost();
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
	const XMVECTOR GetPosture();
	const Vector3 &GetMoveVec();
	float GetBasePlanetScale();
	const std::weak_ptr<Planet> &GetBasePlanet();
	GameDatas::GravityData GetGravityData();
	GameDatas::PlayerStatus GetPlayerStatus();
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

	bool LandingCamReset();

private://�A�j���[�V�����֘A
	//�v���C���[�L�����N�^�[�̕\��e�N�X�`���n���h��
	std::vector<int> faceTextureHandles_;
	//���݂̕\��
	GameDatas::PlayerFaceTexture face_ = GameDatas::PlayerFaceTexture::NORMAL;

	int blinkTimer_ = 300;
	void FaceUpdate();
private:
	Vector3 pos_;
	Object3D drawObject_;
	Object3D leftTrackObject_;
	Object3D rightTrackObject_;

	Object3D shadowObject_;
	Camera *cam_;
	std::weak_ptr<Planet> grabPlanet;

	std::weak_ptr<Planet> basePlanet;

	//�v���C���[�̏��
	GameDatas::PlayerStatus status_;
	//�ʒu�t���[���O�̏��
	GameDatas::PlayerStatus oldStatus_;
	//�d�͂�ύX������
	bool isGravityChanged_ = false;

	DirectX::XMVECTOR playerAngle;

	Vector3 warkVec_;


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
	std::unique_ptr<Model> leftTrackModel_;
	std::unique_ptr<Model> rightTrackModel_;

	std::unique_ptr<Model> ArrowModel_;
};

