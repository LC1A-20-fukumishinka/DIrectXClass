#pragma once
#include "ObjectOnSphere.h"

class Flag : public ObjectOnSphere
{
public:
	Flag();
	~Flag();

	void Init(std::weak_ptr<Planet> basePlanet, Vector3 angle, float scale) override;
	void Update() override;
	void Finalize() override;
	void Draw()override;

	void DrawShadow();

private://staic�ϐ�
	static std::unique_ptr<Model> sFlagModel;
	static Camera *sShadowCamera;
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
	bool CollisionPlayer(float radius, const Vector3 pos);

	bool GetIsDraw();

	void Reset();
private://�����o�ϐ�

	Object3D shadowObject_;

	float worldScale_ = 1.0f;

	bool isDraw_ = true;
};

