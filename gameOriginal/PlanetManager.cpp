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

void PlanetManager::AddPlanet(const DirectX::XMFLOAT3 &pos, float size, const DirectX::XMFLOAT4 &color, int ID, bool isSpawn,const PlanetType &type)
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
		Vector3 newPlanetPos = tmpPlanet->GetPos();

		weak_ptr<Planet> beforePlanet = GetBasePlanet();

		planets.emplace_back(tmpPlanet);

		//惑星間の距離の長さを計算
		Vector3 distance = newPlanetPos - beforePlanet.lock()->GetPos();
		float length = distance.length();

		//それぞれの惑星の半径を引く
		float beforePlanetScale = beforePlanet.lock()->GetStartScale();
		length -= beforePlanetScale;
		length -= tmpPlanet->GetStartScale();


		const float bridgeLength = 25.0f;
		length -= (bridgeLength / 2);
		//惑星同士の間に必要な個数を計算
		int bridgeCount = static_cast<int>(length / bridgeLength);

		bridgeCount++;
		//個数で割って間をつなぐ惑星同士の長さを計算
		length = (length / bridgeCount);

		//距離を正規化して方向を出す
		Vector3 angle = distance.normalize();
		for (int i = 0; i < bridgeCount; i++)
		{
			//開始地点(ひとつ前の惑星)
			Vector3 pos = beforePlanet.lock()->GetPos();

			//半径分移動
			pos += (angle * (beforePlanetScale + (bridgeLength) / 2));

			pos += (angle * (length * i));
			shared_ptr<Planet> bridgePlanet;
			bridgePlanet = make_shared<Planet>();
			bridgePlanet->Init(pos, 10.0f, DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), stage, false);
			//一覧に追加
			planets.emplace_back(bridgePlanet);
		}
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
		bool isBase = e->GetType() == PlanetType::BASE;

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
