#include "PlanetManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "LoadObject.h"
#include "gameConstData.h"
#include <limits.h>
#include "PlanetParticle.h"
#include "../imgui/ImguiManager.h"
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
	//一つ以上ないとバグるので生成
	AddPlanet(XMFLOAT3(0, 0, 0), 50, DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), 0, true, PlanetType::BASE);
	planets_.begin()->get()->SetIsBloom(true);
	planets_.begin()->get()->ColorChange();
}

void PlanetManager::Update()
{
	for (auto &e : planets_)
	{
		e->Update();

		if (e->GetIsFinishedColorChange())
		{
		Vector3 pos = e->GetPos();
		float sclae = e->GetStartScale();
			pPlanetParticles_->AddPlanetParticle(e->GetPos(), e->GetScale(), e->GetColor());
		}
	}

	if (stagePlanets_.size() > 0)
	{
		StageUpdate();
	}

	EditUpdate();
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
	cameraRay.dir = F3toV(angle);
	XMFLOAT3 tmp = pos;
	cameraRay.start = F3toV(tmp);
	Sphere planetCol;

	for (auto &e : planets_)
	{
		planetCol.center = F3toV(e->GetPos());
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

	//全部の惑星の近い惑星をとる
	for (auto &e : planets_)
	{
		//存在しない惑星をスキップ
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
	//全部の惑星の近い惑星をとる
	for (auto &e : planets_)
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
	if (getPlanetNum < planets_.size())
	{
		int planetNum = 0;

		for (auto &e : planets_)
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
	//アニメーション中フラグをオンにする
	isSpawnAnimation_ = true;

	for (auto &e : planets_)
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

	for (auto &e : planets_)
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
		for (auto &e : planets_)
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
	stagePlanets_ = tmp;
}

bool PlanetManager::LoadStage(int stage)
{

	std::vector<shared_ptr<Planet>> tmpPlanet;

	//テキストからロード
	bool isLoad = LoadStageFile(stage, tmpPlanet);

	//ロードに成功したら
	if (isLoad)
	{
		//ロードした惑星を詰め込む
		for (auto &e : tmpPlanet)
		{
			planets_.emplace_back(e);
		}
	}

	return isLoad;
}

void PlanetManager::SetPlanetParticles(PlanetParticle *planetParticles)
{
pPlanetParticles_ = planetParticles;
}

std::shared_ptr<Planet> PlanetManager::GetBasePlanet(int stageNum)
{
	//最終的に返す惑星のポインタ
	shared_ptr<Planet> returnPlanet, lastBase;

	//該当要素が見つからなかった
	bool isNotFound = true;

	for (auto &e : planets_)
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

bool PlanetManager::StageClear()
{
	bool isClear = (orderCount == static_cast<int>(stagePlanets_.size()));

	if (isClear)
	{
		for (auto &e : stagePlanets_)
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
	//カメラの距離の最大値より少し大きく
	float distance = GameDatas::camMaxLength + 1.0f;

	Sphere planetCol;
	for (auto &e : planets_)
	{
	float NearLDistance = (Vector3(cameraRay.start) - e->GetPos()).length();
		NearLDistance -= e->GetScale();
		if (distance > NearLDistance)
		{
			planetCol.center = F3toV(e->GetPos());
			planetCol.radius = e->GetScale();
			Collision::CheckRay2Sphere(cameraRay, planetCol, &distance);
		}
	}

	//めり込んだ際などにバグが発生するのでカメラに影響が出ないように初期値に修正する
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

void PlanetManager::OrderUpdate()
{
	//一つのステージの惑星の数
	int stagePlanetCount = static_cast<int>(stagePlanets_.size());

	//対象なし
	bool isNotFound = true;
	//失敗した
	bool isFailed = false;

	//すべての惑星を確認
	for (auto &e : stagePlanets_)
	{
		
		int count = static_cast<int>(&e - &stagePlanets_[0]);
		if (e.lock() == playerStandPlanet_.lock())
		{
			if (orderCount == count)
			{
				//正しい惑星を踏んだ(次の惑星に)
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

	//次の惑星が光っていない
	bool isNextPlanetNotShining = (orderCount < static_cast<int>(stagePlanets_.size()) && !stagePlanets_[orderCount].lock()->GetIsNext());
	if (isNextPlanetNotShining)
	{
		stagePlanets_[orderCount].lock()->SetNextOrder(true);
	}

	if (isFailed && orderCount > 0)
	{
		for (auto &e : stagePlanets_)
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

void PlanetManager::EditUpdate()
{
	EditMenu();
}

void PlanetManager::EditMenu()
{
	bool makeFlag = false;


	ImGui::Begin("edit");
	ImGui::SetWindowSize(ImVec2(100, 100), ImGuiCond_::ImGuiCond_FirstUseEver);
	makeFlag = ImGui::Button("MakePlanet");

	if (!controllPlanet_.expired())
	{
		//座標を受け取って加工して戻す
		Vector3 pos = controllPlanet_.lock()->GetPos();
		float ImguiPos[3] = { pos.x,pos.y ,pos.z };

		float ImguiScale = controllPlanet_.lock()->GetScale();
		ImGui::InputFloat3("position", ImguiPos);
		ImGui::InputFloat("scale", &ImguiScale);
		pos = Vector3(ImguiPos[0], ImguiPos[1], ImguiPos[2]);
		
		controllPlanet_.lock()->SetPos(pos);
		controllPlanet_.lock()->SetScale(ImguiScale);




		DirectX::XMFLOAT4 color = controllPlanet_.lock()->GetColor();
		float ImguiColor[4] = { color.x,color.y ,color.z , color.w};

		ImGui::SliderFloat4("color", ImguiColor, 0.0f, 1.0f);

		color = DirectX::XMFLOAT4(ImguiColor[0], ImguiColor[1], ImguiColor[2], ImguiColor[3]);
		controllPlanet_.lock()->SetColor(color);
	}

	bool isZero = (makePlanets_.size() <= 0);
	int Num = controllPlanetNumber;
	ImGui::InputInt("controllNumber", &Num);
	bool isChange = (Num != controllPlanetNumber);
	if (!isZero && isChange)
	{
		if (Num < makePlanets_.size() && Num >= 0)
		{
			controllPlanetNumber = Num;

			controllPlanet_ = makePlanets_[Num];
		}
	}

	
	ImGui::End();

	if (makeFlag)
	{
		MakePlanet();
	}
}

void PlanetManager::MakePlanet()
{
	//この瞬間生成されるオブジェクト
	shared_ptr<Planet> makePlanetObject;
	makePlanetObject = make_shared<Planet>();
	int ID = 0;

	//すでに持っている場合
	if (!controllPlanet_.expired())
	{
		Vector3 prePos = controllPlanet_.lock()->GetPos();
		float preScale = controllPlanet_.lock()->GetScale();
		XMFLOAT4 preColor = controllPlanet_.lock()->GetColor();
		ID = controllPlanet_.lock()->GetID();
		makePlanetObject->Init(prePos, preScale, preColor, ID, true, BASE);

		controllPlanetNumber++;
	}
	else
	{
		//一番後ろのID＋１
		ID = planets_.back()->GetID();
		ID += 1;

		makePlanetObject->Init(Vector3(), 50.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), ID, true, BASE);
	}

	controllPlanet_ = makePlanetObject;

	planets_.emplace_back(makePlanetObject);

	makePlanets_.emplace_back(makePlanetObject);

}
