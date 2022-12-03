#include "PlanetManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "LoadPlanet.h"
#include "gameConstData.h"
#include <limits.h>
using namespace std;
using namespace DirectX;
bool PlanetManager::isMakeInstance = false;

PlanetManager::PlanetManager()
{
}

PlanetManager::~PlanetManager()
{
}

std::unique_ptr<PlanetManager> &PlanetManager::Instance()
{
	static unique_ptr<PlanetManager> instance;
	if (!isMakeInstance)
	{
		PlanetManager *makeInstance = new(PlanetManager);
		isMakeInstance = true;
		instance = unique_ptr<PlanetManager>(makeInstance);
	}
	return instance;
}

void PlanetManager::Init()
{
	AddPlanet(XMFLOAT3(0, 0, 0), 50, DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), 0, true, PlanetType::BASE);
	planets_.begin()->get()->SetIsBloom(true);
	planets_.begin()->get()->ColorChange();
}

void PlanetManager::Update()
{
	for (auto &e : planets_)
	{
		e->Update();
	}

	if (stagePlanets.size() > 0)
	{
		StageUpdate();
	}
}

void PlanetManager::Draw()
{
	for (auto &e : planets_)
	{
		e->Draw();
	}
}

void PlanetManager::Finalize()
{
}

void PlanetManager::BloomDraw()
{
	for (auto &e : planets_)
	{
		if (e->GetIsBloom())
		{
			e->Draw();
		}
	}
}

bool PlanetManager::GetGrabPlanet(std::shared_ptr<Planet> &planet, const DirectX::XMFLOAT3 &pos, const DirectX::XMFLOAT3 &angle)
{
	float minDist = 10000.0f;
	bool isCollision = false;
	Ray cameraRay;
	cameraRay.dir = XMLoadFloat3(&angle);
	XMFLOAT3 tmp = pos;
	cameraRay.start = XMLoadFloat3(&tmp);
	Sphere planetCol;

	for (auto &e : planets_)
	{
		planetCol.center = XMLoadFloat3(&e->GetPos());
		planetCol.radius = e->GetScale();
		float colDist;
		if (!e->GetIsOnPlayer() && Collision::CheckRay2Sphere(cameraRay, planetCol, &colDist))
		{
			if (colDist <= minDist)
			{
				minDist = colDist;
				planet = e;
				isCollision = true;
			}
		}
	}
	return isCollision;
}

bool PlanetManager::MovePlanet(std::shared_ptr<Planet> &planet, const DirectX::XMFLOAT3 &pos)
{
	float minDist = 10000.0f;

	//�S���̘f���̋߂��f�����Ƃ�
	for (auto &e : planets_)
	{
		//���݂��Ȃ��f�����X�L�b�v
		if (!e->GetIsSpawn()) { continue; }


		float tmpLength = (e->GetPos() - pos).length();

		tmpLength -= e->GetScale();
		if (tmpLength <= minDist)
		{
			planet = e;
			minDist = tmpLength;
		}
	}
	return !planet->GetIsOnPlayer();
}

Vector3 PlanetManager::GetGravity(const Vector3 pos)
{
	Vector3 gravity;
	//�S���̘f���̋߂��f�����Ƃ�
	for (auto &e : planets_)
	{
		Vector3 dist = (e->GetPos() - pos);

		//���a���̑傫���̋����܂ŋz���񂹂�
		if (dist.length() <= (e->GetScale() * 2))
		{
			//	���͂̋��� = �ʂƂ̋��� / ���̔��a
			float power = (1 - ((dist.length() - e->GetScale()) / e->GetScale()));
			gravity += (dist.normalize() * power);
		}
	}

	if (gravity.length() >= 1.0f)
	{
		gravity = gravity.normalize();
	}

	return gravity;
}

std::weak_ptr<Planet> PlanetManager::GetPlanet(int getPlanetNum)
{
	if (getPlanetNum < planets_.size())
	{
		int planetNum = 0;

		for (auto &e : planets_)
		{
			//�f���̔ԍ�����v�����炻�̘f����Ԃ�
			if (planetNum == getPlanetNum)
			{
				return e;
			}
			//��v���ĂȂ������̂ň�ԍ����₷
			planetNum++;
		}
	}
	//�Ώۂ��Ȃ�������擪�̘f����n��
	return *planets_.begin();
}

void PlanetManager::AddPlanet(const DirectX::XMFLOAT3 &pos, float size, const DirectX::XMFLOAT4 &color, int ID, bool isSpawn, const PlanetType &type)
{
	std::shared_ptr<Planet> planet;
	planet = make_shared<Planet>();
	planet->Init(pos, size, color, ID, isSpawn, type);
	planets_.push_back(planet);
}

void PlanetManager::Reset()
{
	for (auto &e : planets_)
	{
		e->Reset();
	}
}

void PlanetManager::IDSpawn(int ID)
{
	//�A�j���[�V�������t���O���I���ɂ���
	isSpawnAnimation_ = true;

	for (auto &e : planets_)
	{
		int tmpID = e->GetID();
		//���ݑ��݂��Ȃ��f����I��
		if (!e->GetIsSpawn() && (tmpID == ID))
		{
			e->SpawnAnimationStart();
		}
	}
}

void PlanetManager::AllSpawn()
{
	//�A�j���[�V�������t���O���I���ɂ���
	isSpawnAnimation_ = true;

	for (auto &e : planets_)
	{
		//���ݑ��݂��Ȃ��f����I��
		if (!e->GetIsSpawn())
		{
			e->SpawnAnimationStart();
		}
	}
}

bool PlanetManager::SpawnAnimationEnd(int ID)
{
	//�A�j���[�V���������ǂ������m�F
	bool isAnimationEnd = isSpawnAnimation_;

	//�A�j���[�V����������
	if (isAnimationEnd)
	{
		for (auto &e : planets_)
		{
			if (e->GetID() == ID)
			{
				//�A�j���[�V�������I�����Ă�����true
				isAnimationEnd = e->GetIsSpawnAnimationEnd();
				if (isAnimationEnd)
				{
					break;
				}
			}
		}
	}

	//���x�A�j���[�V�������I�����Ă�����
	if (isAnimationEnd)
	{
		//�A�j���[�V�������ł���t���O���I�t�ɂ���
		isSpawnAnimation_ = false;
	}

	return isAnimationEnd;
}

void PlanetManager::SetCamera(Camera *cam)
{
	Planet::SetCamera(cam);
}

void PlanetManager::SetShadowCamera(Camera *cam)
{

	Planet::SetShadowCamera(cam);
}

void PlanetManager::SetShadowTexture(int shadowTextureNum)
{
	Planet::SetShadowTexture(shadowTextureNum);
}

void PlanetManager::SetStagePlanets(int stageNum)
{
	std::vector<std::weak_ptr<Planet>> tmp;
	for (auto &e : planets_)
	{
		if (e->GetID() == stageNum)
		{
			tmp.emplace_back(e);
		}
	}

	orderCount = 0;

	if (tmp.size() <= 0)
	{
		orderCount = 10000;
	}
	stagePlanets = tmp;
}

bool PlanetManager::LoadStage(int stage)
{

	std::vector<shared_ptr<Planet>> tmpPlanet;

	bool isLoad = LoadStageFile(stage, tmpPlanet);

	if (isLoad)
	{
		//���[�h�����f�����l�ߍ���
		for (auto &e : tmpPlanet)
		{
			planets_.emplace_back(e);
		}
	}

	return isLoad;
}

std::shared_ptr<Planet> PlanetManager::GetBasePlanet(int stageNum)
{
	//�ŏI�I�ɕԂ��f���̃|�C���^
	shared_ptr<Planet> returnPlanet, lastBase;

	//�Y���v�f��������Ȃ�����
	bool isNotFound = true;

	for (auto &e : planets_)
	{
		bool isBase = (e->GetType() == PlanetType::BASE);

		if (isBase)
		{
			lastBase = e;
			int tmpID = e->GetID();
			// base��������ID����v���Ă���
			if (tmpID == stageNum)
			{
				returnPlanet = e;
				isNotFound = false;
				break;
			}
		}
	}

	//�v�f��������Ȃ�������Ō�̗v�f���Q��
	if (isNotFound)
	{
		returnPlanet = lastBase;
	}

	return returnPlanet;
}

bool PlanetManager::StageClear()
{
	bool isClear = (orderCount == static_cast<int>(stagePlanets.size()));

	if (isClear)
	{
		for (auto &e : stagePlanets)
		{
			e.lock()->SetIsBloom(true);
		}
		orderCount++;
	}
	return isClear;
}

void PlanetManager::playerStand(std::weak_ptr<Planet> playerStandPlanet)
{
	playerStandPlanet_ = playerStandPlanet;
}

float PlanetManager::CameraCollision(Ray cameraRay)
{
	//�J�����̋����̍ő�l��菭���傫��
	float distance = GameDatas::camMaxLength + 1.0f;

	Sphere planetCol;
	for (auto &e : planets_)
	{
	float NearLDistance = (Vector3(cameraRay.start) - e->GetPos()).length();
		NearLDistance -= e->GetScale();
		if (distance > NearLDistance)
		{
			planetCol.center = XMLoadFloat3(&e->GetPos());
			planetCol.radius = e->GetScale();
			Collision::CheckRay2Sphere(cameraRay, planetCol, &distance);
		}
	}

	//�߂荞�񂾍ۂȂǂɃo�O����������̂ŃJ�����ɉe�����o�Ȃ��悤�ɏ����l�ɏC������
	if (distance <= 0.0f)
	{
		distance = GameDatas::camMaxLength + 1.0f;
	}
	return distance;
}

void PlanetManager::StageUpdate()
{
	OrderUpdate();
}

void PlanetManager::TimeAttackUpdate()
{
	int playerOnPlanet = 0;
	bool check = false;
	//�������^�[��
	for (auto &e : stagePlanets)
	{
		if (!e.lock()->GetIsColorChange())
		{
			return;
		}
		if (!check)
		{
			if (e.lock()->GetIsOnPlayer())
			{
				check = true;
			}
			else
			{
				playerOnPlanet++;
			}
		}

	}


	int max = static_cast<int>(stagePlanets.size());
	int colorOffPlanet = rand() % max;
	while (true)
	{
		if (colorOffPlanet != playerOnPlanet)
		{
			break;
		}
		colorOffPlanet = rand() % max;
	}

	int planetCount = 0;
	for (auto &e : stagePlanets)
	{
		if (planetCount == colorOffPlanet)
		{
			score++;
			e.lock()->ColorOff();
			break;
		}
		else
		{
			planetCount++;
		}
	}
}

void PlanetManager::OrderUpdate()
{
	//��̃X�e�[�W�̘f���̐�
	int stagePlanetCount = static_cast<int>(stagePlanets.size());

	//�ΏۂȂ�
	bool isNotFound = true;
	//���s����
	bool isFailed = false;

	//���ׂĂ̘f�����m�F
	for (auto &e : stagePlanets)
	{
		
		int count = static_cast<int>(&e - &stagePlanets[0]);
		if (e.lock() == playerStandPlanet_.lock())
		{
			if (orderCount == count)
			{
				//�������f���𓥂�(���̘f����)
				e.lock()->ColorChange();
				e.lock()->SetNextOrder(false);
				orderCount++;
			}
			else if (orderCount < count)
			{
				isFailed = true;
			}
			isNotFound = false;
		}
	}

	//���̘f���������Ă��Ȃ�
	bool isNextPlanetNotShining = (orderCount < static_cast<int>(stagePlanets.size()) && !stagePlanets[orderCount].lock()->GetIsNext());
	if (isNextPlanetNotShining)
	{
		stagePlanets[orderCount].lock()->SetNextOrder(true);
	}

	if (isFailed && orderCount > 0)
	{
		for (auto &e : stagePlanets)
		{
			e.lock()->ColorOff();
			e.lock()->SetNextOrder(false);
		}
		orderCount = 0;
	}

	if (isNotFound)
	{
		playerStandPlanet_.lock()->ColorChange();
	}
}
