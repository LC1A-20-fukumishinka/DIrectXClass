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
	/// Šî–{‚Ì˜f¯‚ğó‚¯“n‚·
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<Planet> GetBasePlanet();
	void AddPlanet(const DirectX::XMFLOAT3 &pos, float size, const DirectX::XMFLOAT4 &color);
private:
	std::list<std::shared_ptr<Planet>> planets;

	static bool isMakeInstance;
};

