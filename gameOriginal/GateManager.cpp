#include "GateManager.h"
#include "LoadObject.h"
#include "GateParticle.h"

GateManager::GateManager()
{

}

GateManager::~GateManager()
{
}

void GateManager::Init()
{
	for (int i = 0; i < 5; i++)
	{
		float height = 50;
		std::unique_ptr<Gate> gate;
		gate = std::make_unique<Gate>();
		gate->Init(Vector3(-50, height, 100.0f + i * 50.0f), Vector3(0.0f, 0.0f, 1.0f), 0, true);
		gates_.emplace_back(gate.get());
		gate.release();

		std::unique_ptr<Gate> gateD;
		gateD = std::make_unique<Gate>();
		gateD->Init(Vector3(+50, height, 100.0f + i * 50.0f), Vector3(0.0f, 0.0f, 1.0f), 0, true);
		gates_.emplace_back(gateD.get());
		gateD.release();
	}

	LoadStage(1);
}

void GateManager::Update()
{
	//プレイヤーが立ち丈太になっていたらゲートを復活させる
	bool isReborn = playerStatus_ == GameDatas::PlayerStatus::STAND;
	for (auto& e : gates_)
	{
		e->Update();
		if (isReborn)
		{
			e->Reborn();
		}
	}
}

void GateManager::Draw()
{
	for (auto& e : gates_)
	{
		e->Draw();
	}
}

void GateManager::Finalize()
{
}

void GateManager::Reset()
{
	for (auto& e : gates_)
	{
		e->Reset();
	}
}

bool GateManager::LoadStage(int stage)
{
	std::vector<std::unique_ptr<Gate>> tmp;

	bool isLoad = LoadGateFile(stage, tmp);

	if (isLoad)
	{
		for (auto& e : tmp)
		{
			gates_.emplace_back(e.get());
			e.release();
		}
	}

	return isLoad;
}

bool GateManager::Collision(const Vector3& pos, float speed)
{
	Sphere playerS;
	playerS.center = F3toV(pos);
	playerS.radius = speed;
	bool isCollision = false;
	for (auto& e : gates_)
	{
		if (e->Collision(playerS))
		{
			isCollision = true;
			gateParticles_->AddGateParticle(e->GetAngle());
		}
	}
	return isCollision;
}

void GateManager::SetGateParticle(GateParticle* gateParticle)
{
	gateParticles_ = gateParticle;
}

void GateManager::ReceivePlayerStatus(const GameDatas::PlayerStatus& playerStatus)
{
	playerStatus_ = playerStatus;
}
