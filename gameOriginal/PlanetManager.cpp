#include "PlanetManager.h"
#include "../Collision/Collision.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "LoadPlanet.h"
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
}

void PlanetManager::Update()
{
	for (auto &e : planets)
	{
		e->Update();
	}
}

void PlanetManager::Draw()
{
	for (auto &e : planets)
	{
		e->Draw();
	}
}

void PlanetManager::Finalize()
{
}

bool PlanetManager::GetGrabPlanet(std::shared_ptr<Planet> &planet, const DirectX::XMFLOAT3 &pos, const DirectX::XMFLOAT3 &angle)
{
	float minDist = 10000.0f;
	bool isCollision = false;
	Ray cameraRay;
	cameraRay.dir = XMLoadFloat3(&angle);
	XMFLOAT3 tmp = pos;
	cameraRay.start = XMLoadFloat3(&tmp);
	Sphere starCol;

	for (auto &e : planets)
	{
		starCol.center = XMLoadFloat3(&e->GetPos());
		starCol.radius = e->GetScale();
		float colDist;
		if (!e->GetBase() && Collision::CheckRay2Sphere(cameraRay, starCol, &colDist))
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
	for (auto &e : planets)
	{
		//���݂��Ȃ��f�����X�L�b�v
		if (!e->GetIsSpawn()) {continue;}


		float tmpLength = (e->GetPos() - pos).length();

		tmpLength -= e->GetScale();
		if (tmpLength <= minDist)
		{
			planet = e;
			minDist = tmpLength;
		}
	}
	return !planet->GetBase();
}

Vector3 PlanetManager::GetGravity(const Vector3 pos)
{
	Vector3 gravity;
	//�S���̘f���̋߂��f�����Ƃ�
	for (auto &e : planets)
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
	if (getPlanetNum < planets.size())
	{
		int planetNum = 0;

		for (auto &e : planets)
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
	return *planets.begin();
}

void PlanetManager::AddPlanet(const DirectX::XMFLOAT3 &pos, float size, const DirectX::XMFLOAT4 &color, int ID, bool isSpawn, const PlanetType &type)
{
	std::shared_ptr<Planet> planet;
	planet = make_shared<Planet>();
	planet->Init(pos, size, color, ID, isSpawn, type);
	planets.push_back(planet);
}

void PlanetManager::Reset()
{
	for (auto &e : planets)
	{
		e->Reset();
	}
}

void PlanetManager::IDSpawn(int ID)
{
	//�A�j���[�V�������t���O���I���ɂ���
	isSpawnAnimation_ = true;

	for (auto &e : planets)
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

	for (auto &e : planets)
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
		for (auto &e : planets)
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

bool PlanetManager::LoadStage(int stage)
{

	shared_ptr<Planet> tmpPlanet;

	bool isLoad = LoadStageFile(stage, tmpPlanet);

	if (isLoad)
	{
		planets.emplace_back(tmpPlanet);
	}

	return isLoad;
}

std::shared_ptr<Planet> PlanetManager::GetBasePlanet(int stageNum)
{
	//�ŏI�I�ɕԂ��f���̃|�C���^
	shared_ptr<Planet> returnPlanet, lastBase;

	//�Y���v�f��������Ȃ�����
	bool isNotFound = true;

	for (auto &e : planets)
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
