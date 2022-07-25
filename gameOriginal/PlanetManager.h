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
	/// 指定した番号の惑星を返す関数（とりあえずで作った関数、テスト用）
	/// </summary>
	/// <param name="getPlanetNum">番号</param>
	/// <returns>指定された惑星（存在しない番号の場合は先頭の番号を返す）</returns>
	std::weak_ptr<Planet> GetPlanet(int getPlanetNum);
	/// <summary>
	/// 基本の惑星を受け渡す
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<Planet> GetBasePlanet();
	void AddPlanet(const DirectX::XMFLOAT3 &pos, float size, const DirectX::XMFLOAT4 &color);

public:
	void SetCamera(Camera *cam);
	void SetShadowCamera(Camera *cam);
	void SetShadowTexture(int shadowTextureNum);
private:
	std::list<std::shared_ptr<Planet>> planets;

	static bool isMakeInstance;
};

