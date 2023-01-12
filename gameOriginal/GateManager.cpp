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

	int stageNum = 0;

	//���ׂẴX�e�[�W��ǂݍ���
	while(LoadStage(stageNum))
	{
		stageNum++;
	}
}

void GateManager::Update()
{
	//�v���C���[�������䑾�ɂȂ��Ă�����Q�[�g�𕜊�������
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

GameDatas::CollisionGateData GateManager::Collision(const Vector3& pos, float speed)
{

	GameDatas::CollisionGateData retData;

	Sphere playerS;
	playerS.center = F3toV(pos);
	playerS.radius = speed;
	for (auto& e : gates_)
	{
		if (e->Collision(playerS))
		{
			retData.isCollision = true;
			retData.color = e->GetColor();
			gateParticles_->AddGateParticle(e->GetAngle(), retData.color);
		}
	}
	return retData;
}

void GateManager::SetGateParticle(GateParticle* gateParticle)
{
	gateParticles_ = gateParticle;
}

void GateManager::ReceivePlayerStatus(const GameDatas::PlayerStatus& playerStatus)
{
	playerStatus_ = playerStatus;
}

void GateManager::GateSpawn(int stageNum)
{
	for (auto& e : gates_)
	{
		if (e->GetID() == stageNum)
		{
			e->Spawn();
		}
	}
}

const std::vector<std::shared_ptr<Gate>>& GateManager::GetGates()
{
	return gates_;
}

std::weak_ptr<Gate> GateManager::MakeGate(std::weak_ptr<Gate>& controllGate, int stageNum)
{
	//���̏u�Ԑ��������I�u�W�F�N�g
	std::shared_ptr<Gate> makeGateObject;
	makeGateObject = std::make_shared<Gate>();
	int ID = 0;

	//���łɎ����Ă���ꍇ

		//���W���R�s�[���邽�߂ɕK�v
	if (!controllGate.expired())
	{
		//�ǉ��쐬���̃I�u�W�F�N�g�̏�񂩂�V�K�쐬
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
	//�ҏW�z��N���A
	makeGates.clear();
	for (auto& e : gates_)
	{
		//�Ώۂ̘f�������W
		if (e->GetID() == editStageNum)
		{
			makeGates.emplace_back(e);
		}
	}

	//�Ώۂ̘f�������݂������ǂ���
	if (static_cast<int>(makeGates.size()) > 0)
	{//�Ώۂ̘f�������݂�����
		//��Ԍ��̘f���𑀍�ΏۂɂƂ�
		controllGate = makeGates.back();
		return true;
	}
	else
	{//�Ώۂ̘f�������݂��Ȃ�������
		//�{���ΏۂȂ��ɂ���
		controllGate.reset();
		return false;
	}
}

void GateManager::AllSpawn()
{
	for (auto& e : gates_)
	{
		e->Spawn();
	}
}
