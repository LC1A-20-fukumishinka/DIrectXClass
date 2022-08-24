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

	PlanetManager(PlanetManager &obj) = delete;
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
	/// <summary>
	/// ��{�̘f�����󂯓n��
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<Planet> GetBasePlanet();
	void AddPlanet(const DirectX::XMFLOAT3 &pos, float size, const DirectX::XMFLOAT4 &color, bool isSpawn = true);

	void Reset();

	/// <summary>
	/// ���݂��Ȃ���ԂɂȂ��Ă���f�����o��������
	/// </summary>
	void AllSpawn();

	/// <summary>
	/// �A�j���[�V�������I�������u�Ԃ̂�true��Ԃ�
	/// </summary>
	/// <returns>�A�j���[�V�������I�������t���O</returns>
	bool SpawnAnimationEnd();
public:
	void SetCamera(Camera *cam);
	void SetShadowCamera(Camera *cam);
	void SetShadowTexture(int shadowTextureNum);
private:
	std::list<std::shared_ptr<Planet>> planets;

	bool isSpawnAnimation_ = false;

	static bool isMakeInstance;

};

