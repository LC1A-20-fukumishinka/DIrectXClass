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

	//全部の惑星の近い惑星をとる
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
			//惑星の番号が一致したらその惑星を返す
			if (planetNum == getPlanetNum)
			{
				return e;
			}
			//一致してなかったので一つ番号増やす
			planetNum++;
		}
	}
	//対象がなかったら先頭の惑星を渡す
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
	//アニメーション中フラグをオンにする
	isSpawnAnimation_ = true;

	for (auto &e : planets)
	{
		//現在存在しない惑星を選択
		if (!e->GetIsSpawn())
		{
			e->SpawnAnimationStart();
		}
	}
}

bool PlanetManager::SpawnAnimationEnd()
{
	//アニメーション中かどうかを確認
	bool isAnimationEnd = isSpawnAnimation_;

	//アニメーション中かつ
	if (isAnimationEnd)
	{
		for (auto &e : planets)
		{
			//アニメーションが終了していたらtrue
			isAnimationEnd = e->GetIsSpawnAnimationEnd();
			if (isAnimationEnd)
			{
				break;
			}

		}
	}

	//丁度アニメーションが終了していたら
	if (isAnimationEnd)
	{
		//アニメーション中であるフラグをオフにする
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
