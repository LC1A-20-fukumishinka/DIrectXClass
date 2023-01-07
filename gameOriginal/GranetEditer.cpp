#include "GranetEditer.h"
#include "../imgui/ImguiManager.h"
#include "PlanetManager.h"
#include "GateManager.h"
#include "LoadObject.h"
GranetEditer::GranetEditer()
{
}
GranetEditer::~GranetEditer()
{
}
void GranetEditer::EditUpdate()
{
	//�ǂ������̃}�l�[�W���[�����݂��Ȃ�������߂�
	if (planetManager_ == nullptr || gateManager_ == nullptr) return;
	EditMenu();
}

void GranetEditer::EditMenu()
{
	ImGui::Begin("edit");
	bool setFlag = false;

	ImGui::InputInt("stageNumber", &editStageNum);
	setFlag = ImGui::Button("SetStage");

	//�w�肳�ꂽ�ԍ��̃X�e�[�W��ҏW�z��ɓ����
	if (setFlag)
	{
		bool isSetPlanet = planetManager_->SetMakePlanet(editStageNum, makePlanets_, controllPlanet_);
		if (isSetPlanet)
		{
			//����ԍ����X�V����
			controllPlanetNumber = (static_cast<int>(makePlanets_.size()) - 1);
		}
		else
		{
			//�V�������������X�e�[�W�̘f���ԍ�
			controllPlanetNumber = 0;
		}

		bool isSetGate = gateManager_->SetMakeGates(editStageNum, makeGates_, controllGate_);
		if (isSetGate)
		{
			//����ԍ����X�V����
			controllGateNumber = (static_cast<int>(makeGates_.size()) - 1);
		}
	}
	PlanetEditMenu();
	GateEditMenu();

	bool Save = false;
	bool AllSave = false;
	Save = ImGui::Button("Save");
	AllSave = ImGui::Button("AllStageSave");

	if (Save)
	{
		SaveStageFile(makePlanets_, makeGates_, false);
	}

	if (AllSave)
	{
		SaveAllStageFile(planetManager_->GetPlanets(), gateManager_->GetGates());
	}
	ImGui::End();

}

void GranetEditer::GateEditMenu()
{
	bool makeFlag = false;

	makeFlag = ImGui::Button("MakeGate");

	//����Ώۂ����݂��Ȃ�������\�����Ȃ�
	if (!controllGate_.expired())
	{


		//���W���󂯎���ĉ��H���Ė߂�
		Vector3 pos = controllGate_.lock()->GetPos();
		Vector3 angle = controllGate_.lock()->GetAngle();
		float ImguiPos[3] = { pos.x,pos.y ,pos.z };
		float ImguiAngle[3] = { angle.x,angle.y ,angle.z };
		ImGui::DragFloat3("GatePosition", ImguiPos, 0.5f);
		ImGui::SliderFloat3("GateAngle", ImguiAngle, -1.0f, 1.0f);

		pos = Vector3(ImguiPos[0], ImguiPos[1], ImguiPos[2]);
		angle = Vector3(ImguiAngle[0], ImguiAngle[1], ImguiAngle[2]);
		controllGate_.lock()->SetPos(pos);
		controllGate_.lock()->SetAngle(angle);



		DirectX::XMFLOAT4 color = controllGate_.lock()->GetColor();
		float ImguiColor[4] = { color.x,color.y ,color.z , color.w };

		ImGui::SliderFloat4("gateColor", ImguiColor, 0.0f, 1.0f);

		color = DirectX::XMFLOAT4(ImguiColor[0], ImguiColor[1], ImguiColor[2], ImguiColor[3]);
		controllGate_.lock()->SetColor(color);


	}

	//����ΏەύX
	bool isZero = (makeGates_.size() <= 0);
	int Num = controllGateNumber;
	ImGui::InputInt("controllGateNumber", &Num);

	bool isChange = (Num != controllGateNumber);

	if (!isZero && isChange)
	{
		//�Ώۂ���
		if (Num < makeGates_.size() && Num >= 0)
		{
			controllGateNumber = Num;

			controllGate_ = makeGates_[Num];
		}
	}




	if (makeFlag)
	{
		controllGate_ = gateManager_->MakeGate(controllGate_, editStageNum);

		//����X�e�[�W�̈ꗗ�f�[�^�ɒǉ�
		makeGates_.emplace_back(controllGate_);

		controllGateNumber = (static_cast<int>(makeGates_.size()) - 1);
	}
}

void GranetEditer::PlanetEditMenu()
{
	bool makeFlag = false;

	ImGui::SetWindowSize(ImVec2(100, 100), ImGuiCond_::ImGuiCond_FirstUseEver);

	makeFlag = ImGui::Button("MakePlanet");

	if (!controllPlanet_.expired())
	{


		//���W���󂯎���ĉ��H���Ė߂�
		Vector3 pos = controllPlanet_.lock()->GetPos();
		float ImguiPos[3] = { pos.x,pos.y ,pos.z };

		float ImguiScale = controllPlanet_.lock()->GetScale();
		ImGui::DragFloat3("PlanetPosition", ImguiPos, 0.5f);
		ImGui::DragFloat("PlanetScale", &ImguiScale, 0.05f);
		pos = Vector3(ImguiPos[0], ImguiPos[1], ImguiPos[2]);

		controllPlanet_.lock()->SetPos(pos);
		controllPlanet_.lock()->SetScale(ImguiScale);




		DirectX::XMFLOAT4 color = controllPlanet_.lock()->GetColor();
		float ImguiColor[4] = { color.x,color.y ,color.z , color.w };

		ImGui::SliderFloat4("color", ImguiColor, 0.0f, 1.0f);

		color = DirectX::XMFLOAT4(ImguiColor[0], ImguiColor[1], ImguiColor[2], ImguiColor[3]);
		controllPlanet_.lock()->SetColor(color);


	}

	bool isZero = (makePlanets_.size() <= 0);
	int Num = controllPlanetNumber;
	ImGui::InputInt("controllPlanetNumber", &Num);
	bool isChange = (Num != controllPlanetNumber);
	if (!isZero && isChange)
	{
		if (Num < makePlanets_.size() && Num >= 0)
		{
			controllPlanetNumber = Num;

			controllPlanet_ = makePlanets_[Num];
		}
	}

	if (makeFlag)
	{
		controllPlanet_ = planetManager_->MakePlanet(controllPlanet_);

		//����X�e�[�W�̈ꗗ�f�[�^�ɒǉ�
		makePlanets_.emplace_back(controllPlanet_);

		controllPlanetNumber = (static_cast<int>(makePlanets_.size()) - 1);
	}
}

void GranetEditer::SetManagers(PlanetManager* planetManager, GateManager* gateManager)
{
	planetManager_ = planetManager;
	gateManager_ = gateManager;
}
