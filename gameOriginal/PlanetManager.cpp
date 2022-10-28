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

	//全部の惑星の近い惑星をとる
	for (auto &e : planets)
	{
		//存在しない惑星をスキップ
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
	//全部の惑星の近い惑星をとる
	for (auto &e : planets)
	{
		Vector3 dist = (e->GetPos() - pos);

		//半径分の大きさの距離まで吸い寄せる
		if (dist.length() <= (e->GetScale() * 2))
		{
			//	引力の強さ = 面との距離 / 星の半径
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
	//アニメーション中フラグをオンにする
	isSpawnAnimation_ = true;

	for (auto &e : planets)
	{
		int tmpID = e->GetID();
		//現在存在しない惑星を選択
		if (!e->GetIsSpawn() && (tmpID == ID))
		{
			e->SpawnAnimationStart();
		}
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

bool PlanetManager::SpawnAnimationEnd(int ID)
{
	//アニメーション中かどうかを確認
	bool isAnimationEnd = isSpawnAnimation_;

	//アニメーション中かつ
	if (isAnimationEnd)
	{
		for (auto &e : planets)
		{
			if (e->GetID() == ID)
			{
				//アニメーションが終了していたらtrue
				isAnimationEnd = e->GetIsSpawnAnimationEnd();
				if (isAnimationEnd)
				{
					break;
				}
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
	//最終的に返す惑星のポインタ
	shared_ptr<Planet> returnPlanet, lastBase;

	//該当要素が見つからなかった
	bool isNotFound = true;

	for (auto &e : planets)
	{
		bool isBase = (e->GetType() == PlanetType::BASE);

		if (isBase)
		{
			lastBase = e;
			int tmpID = e->GetID();
			// base属性かつIDが一致している
			if (tmpID == stageNum)
			{
				returnPlanet = e;
				isNotFound = false;
				break;
			}
		}
	}

	//要素が見つからなかったら最後の要素を参照
	if (isNotFound)
	{
		returnPlanet = lastBase;
	}

	return returnPlanet;
}
