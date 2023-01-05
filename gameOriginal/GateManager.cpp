#include "GateManager.h"
#include "LoadObject.h"
#include "GateParticle.h"
#include "../imgui/ImguiManager.h"
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
		std::shared_ptr<Gate> gate;
		gate = std::make_shared<Gate>();
		gate->Init(Vector3(-50, height, 100.0f + i * 50.0f), Vector3(0.0f, 0.0f, 1.0f), 0, true);
		gates_.emplace_back(gate);

		std::shared_ptr<Gate> gateD;
		gateD = std::make_shared<Gate>();
		gateD->Init(Vector3(+50, height, 100.0f + i * 50.0f), Vector3(0.0f, 0.0f, 1.0f), 0, true);
		gates_.emplace_back(gateD);
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
	std::vector<std::shared_ptr<Gate>> tmp;

	bool isLoad = LoadGateFile(stage, tmp);

	if (isLoad)
	{
		for (auto& e : tmp)
		{
			gates_.emplace_back(e);
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

const std::vector<std::shared_ptr<Gate>>& GateManager::GetGates()
{
	return gates_;
}

std::weak_ptr<Gate> GateManager::MakeGate(std::weak_ptr<Gate>& controllGate, int stageNum)
{
	//この瞬間生成されるオブジェクト
	std::shared_ptr<Gate> makeGateObject;
	makeGateObject = std::make_shared<Gate>();
	int ID = 0;

	//すでに持っている場合

		//座標をコピーするために必要
	if (!controllGate.expired())
	{
		//追加作成元のオブジェクトの情報から新規作成
		Vector3 prePos = controllGate.lock()->GetPos();
		Vector3 preAngle = controllGate.lock()->GetAngle();
		DirectX::XMFLOAT4 preColor = controllGate.lock()->GetColor();
		ID = controllGate.lock()->GetID();
		makeGateObject->Init(prePos, preAngle, static_cast<int>(controllGate.lock()->GetID()), true);
	}
	else
	{
		makeGateObject->Init(Vector3(), Vector3(0.0f, 0.0f, 1.0f), stageNum, true);
	}

	gates_.emplace_back(makeGateObject);

	return makeGateObject;
}

bool GateManager::SetMakeGates(int &editStageNum, std::vector<std::weak_ptr<Gate>>& makeGates, std::weak_ptr<Gate>& controllGate)
{
	//編集配列クリア
	makeGates.clear();
	for (auto& e : gates_)
	{
		//対象の惑星を収集
		if (e->GetID() == editStageNum)
		{
			makeGates.emplace_back(e);
		}
	}

	//対象の惑星が存在したかどうか
	if (static_cast<int>(makeGates.size()) > 0)
	{//対象の惑星が存在したら
		//一番後ろの惑星を操作対象にとる
		controllGate = makeGates.back();
		return true;
	}
	else
	{//対象の惑星が存在しなかったら
		//捜査対象なしにして
		controllGate.reset();
		return false;
	}
}
