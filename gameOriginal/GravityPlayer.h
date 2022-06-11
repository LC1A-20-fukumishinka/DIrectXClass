#pragma once
#include "Object3D.h"
#include "Vector3.h"
#include "Model.h"
#include <memory>
class Planet;
class GravityPlayer
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;
public://��{�֐�

	/// <summary>
	/// �������֐�
	/// </summary>
	/// <param name="model"></param>
	void Init(Model *model);
	void Update();
	void Finalize();
	void Draw();

public://�Q�b�^�Z�b�^
	void SetPos(const DirectX::XMFLOAT3 &pos);

	void SetRotation(const DirectX::XMFLOAT3 &rot);
	void SetModel(Model *model);
	void SetCamera(Camera *cam);
	void SetLight(LightGroup *lights);
	const XMFLOAT3 GetPos();

public://�f������̏���
	/// <summary>
	/// �͂ޘf���̃Z�b�g���s��
	/// </summary>
	/// <param name="planet">�͂ޘf���̃|�C���^</param>
	void SetGrabPlanet(std::shared_ptr<Planet> planet);

	/// <summary>
	/// �͂�ł���f���𗣂�
	/// </summary>
	void ReleasePlanet();

	//�͂�ł���ۂ̍X�V�����Ђ���Ƃ�����v��Ȃ�����
	void GrabUpdate();
private:
	Vector3 pos;
	Vector3 rotation;
	Vector3 scale;
	Object3D drawObject;
	std::weak_ptr<Planet> grabPlanet;
};

