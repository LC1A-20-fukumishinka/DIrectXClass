#pragma once
#include "Planet.h"
#include <list>
#include <memory>
class PlanetManager
{
private:
	PlanetManager();
public:
	~PlanetManager();

	PlanetManager(const PlanetManager &obj) = delete;
	PlanetManager &operator=(const PlanetManager &obj) = delete;

	static std::unique_ptr<PlanetManager> &Instance();

	void Init();
	void Update();
	void Draw();
	void Finalize();
public:
	bool GetGrabPlanet(std::shared_ptr<Planet> &planet, const DirectX::XMFLOAT3 &pos, const DirectX::XMFLOAT3 &angle);


	bool MovePlanet(std::shared_ptr<Planet> &planet, const DirectX::XMFLOAT3 &pos);

	/// <summary>
	/// �w�肵���ԍ��̘f����Ԃ��֐��i�Ƃ肠�����ō�����֐��A�e�X�g�p�j
	/// </summary>
	/// <param name="getPlanetNum">�ԍ�</param>
	/// <returns>�w�肳�ꂽ�f���i���݂��Ȃ��ԍ��̏ꍇ�͐擪�̔ԍ���Ԃ��j</returns>
	std::weak_ptr<Planet> GetPlanet(int getPlanetNum);

	void AddPlanet(const DirectX::XMFLOAT3 &pos, float size, const DirectX::XMFLOAT4 &color, int ID,  bool isSpawn = true, const PlanetType &type = BRIDGE);

	void Reset();

	/// <summary>
	/// �f���̎���ID���Q�Ƃ��ďo��������
	/// </summary>
	/// <param name="ID">�o��������</param>
	void IDSpawn(int ID);
	/// <summary>
	/// ���݂��Ȃ���ԂɂȂ��Ă���f�����o��������
	/// </summary>
	void AllSpawn();

	/// <summary>
	/// �A�j���[�V�������I�������u�Ԃ̂�true��Ԃ�
	/// </summary>
	/// <returns>�A�j���[�V�������I�������t���O</returns>
	bool SpawnAnimationEnd(int ID);
public:
	void SetCamera(Camera *cam);
	void SetShadowCamera(Camera *cam);
	void SetShadowTexture(int shadowTextureNum);

	bool LoadStage(int stage);

	/// <summary>
	/// �w��̔ԍ���base�����̘f���̃|�C���^���擾����
	/// </summary>
	/// <param name="stageNum">�ԍ��̎w��i�Ȃ��ꍇ��͈͊O���w�肵�Ă���ꍇ��Ԍ��̘f����Ԃ��j</param>
	/// <returns>�f���̃|�C���^</returns>
	std::shared_ptr<Planet> GetBasePlanet(int stageNum = -1);
private:
	std::list<std::shared_ptr<Planet>> planets;

	bool isSpawnAnimation_ = false;

	static bool isMakeInstance;

};

