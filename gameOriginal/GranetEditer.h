#pragma once
#include <memory>
#include <vector>
class Planet;
class Gate;
class GateManager;
class PlanetManager;
class Camera;
class GranetEditer
{
public:
	GranetEditer();
	~GranetEditer();
	//�G�f�B�^�p�X�V
	void EditUpdate();

	void SetManagers(PlanetManager* planetManager, GateManager* gateManager, Camera* gameCamera);
private:
	//Imgui�ɂ��G�f�B�b�g���j��-
	void EditMenu();

	void GateEditMenu();

	void PlanetEditMenu();

	std::vector<std::weak_ptr<Planet>> makePlanets_;

	std::weak_ptr<Planet> controllPlanet_;

	int controllPlanetNumber = 0;

	int editStageNum = 0;

	std::vector<std::weak_ptr<Gate>> makeGates_;

	std::weak_ptr<Gate> controllGate_;

	int controllGateNumber = 0;

	GateManager *gateManager_ = nullptr;
	PlanetManager *planetManager_ = nullptr;
	Camera* cam_ = nullptr;
};

