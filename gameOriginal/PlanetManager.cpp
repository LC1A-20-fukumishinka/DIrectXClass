#include "PlanetManager.h"
#include "../Collision/Collision.h"
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
	AddPlanet(XMFLOAT3(0, 0, 0), 50, DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), true);
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
		float tmpDist = (e->GetPos() - pos).length();

		tmpDist -= e->GetScale();
		if (tmpDist <= minDist)
		{
			planet = e;
			minDist = tmpDist;
		}
	}
	return !planet->GetBase();
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

std::shared_ptr<Planet> PlanetManager::GetBasePlanet()
{
	return *planets.begin();
}

void PlanetManager::AddPlanet(const DirectX::XMFLOAT3 &pos, float size, const DirectX::XMFLOAT4 &color, bool isSpawn)
{
	std::shared_ptr<Planet> planet;
	planet = make_shared<Planet>();
	planet->Init(pos, size, color, isSpawn);
	int hoge = 0;
	planets.push_back(planet);
}

void PlanetManager::Reset()
{
	for (auto &e : planets)
	{
		e->Reset();
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

bool PlanetManager::SpawnAnimationEnd()
{
	//�A�j���[�V���������ǂ������m�F
	bool isAnimationEnd = isSpawnAnimation_;

	//�A�j���[�V����������
	if (isAnimationEnd)
	{
		for (auto &e : planets)
		{
			//�A�j���[�V�������I�����Ă�����true
			isAnimationEnd = e->GetIsSpawnAnimationEnd();
			if (isAnimationEnd)
			{
				break;
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
