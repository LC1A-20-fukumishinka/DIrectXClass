#pragma once
#include "Object3D.h"
#include <memory>
#include "Vector3.h"
#include "IGraphicsPipeline.h"

class Model;
class Planet;

// ���ݕ����Ă�����
//
//�E���̎p�����o�O��




/// <summary>
/// ��
/// </summary>
class Flag 
{
public:
	Flag();
	~Flag();

	void Init(std::weak_ptr<Planet> basePlanet,const Vector3 &angle, float scale);
	void Update() ;
	void Finalize() ;
	void Draw();

	void DrawShadow();

private://staic�ϐ�
	static std::unique_ptr<Model> sFlagModel;
	static std::unique_ptr<Model> sLightPillarModel;
	static Camera *sShadowCamera;
	//�Q�[���̃��C���J����
	static Camera *sMainCamera;
	//�Q�[���̃��C�g
	static LightGroup *sLights;

	static int testTex;
	static std::unique_ptr<PipeClass::PipelineSet> sLightPipeline;
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

void LightPillarUpdate();
public://static�֐�


	/// <summary>
	/// �V���h�E�J�����̃Z�b�^
	/// </summary>
	/// <param name="camera">���蓖�Ă�J�����̃|�C���^(���C���J�������l�X�}�[�g�|�C���^������)</param>
	static void SetShadowCamera(Camera *camera);

public:

	/// <summary>
	/// Flag���l�����邽�߂̊֐�
	/// </summary>
	/// <param name="radius">���a</param>
	/// <param name="pos">���W</param>
	/// <returns>�ڐG�������ǂ���</returns>
	bool CollisionPlayer(float radius, const Vector3 &pos);

	bool GetIsDraw();

	void Reset();
private://�����o�ϐ�

	Object3D shadowObject_;
	Object3D lightPillarObject_;
	bool isDraw_ = true;

	Object3D object_;

	std::weak_ptr<Planet> basePlanet_;

	Vector3 upVec_ = { 0, 1, 0 };
	Vector3 frontVec_ = { 0, 0, 1 };
	float worldScale_ = 0.0f;
	Microsoft::WRL::ComPtr<ID3D12Resource> lightConstBuff2;
	float lightRate_ = 1.0f;

	bool lightAnimationFlag_ = true;

	float lightAnimationRate_ = 0.0f;
};

