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
