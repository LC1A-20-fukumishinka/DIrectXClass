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
	//��ȏ�Ȃ��ƃo�O��̂Ő���
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
	stagePlanets_ = tmp;
}

bool PlanetManager::LoadStage(int stage)
{

	std::vector<shared_ptr<Planet>> tmpPlanet;

	//�e�L�X�g���烍�[�h
	bool isLoad = LoadStageFile(stage, tmpPlanet);

	//���[�h�ɐ���������
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

void PlanetManager::SetPlanetParticles(PlanetParticle *planetParticles)
{
pPlanetParticles_ = planetParticles;
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
	//�J�����̋����̍ő�l��菭���傫��
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

void PlanetManager::OrderUpdate()
{
	//��̃X�e�[�W�̘f���̐�
	int stagePlanetCount = static_cast<int>(stagePlanets_.size());

	//�ΏۂȂ�
	bool isNotFound = true;
	//���s����
	bool isFailed = false;

	//���ׂĂ̘f�����m�F
	for (auto &e : stagePlanets_)
	{
		
		int count = static_cast<int>(&e - &stagePlanets_[0]);
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
		//���W���󂯎���ĉ��H���Ė߂�
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
	//���̏u�Ԑ��������I�u�W�F�N�g
	shared_ptr<Planet> makePlanetObject;
	makePlanetObject = make_shared<Planet>();
	int ID = 0;

	//���łɎ����Ă���ꍇ
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
		//��Ԍ���ID�{�P
		ID = planets_.back()->GetID();
		ID += 1;

		makePlanetObject->Init(Vector3(), 50.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), ID, true, BASE);
	}

	controllPlanet_ = makePlanetObject;

	planets_.emplace_back(makePlanetObject);

	makePlanets_.emplace_back(makePlanetObject);

}
