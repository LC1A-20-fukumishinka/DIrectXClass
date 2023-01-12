#include "LoadObject.h"
#include "Vector3.h"
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;
using namespace DirectX;


struct MakePlanetData
{
	Vector3 pos;
	float scale = 0.0f;
};

struct MakeGateData
{
	Vector3 pos;
	Vector3 angle;
};
bool LoadStageFile(int stage, std::vector<std::shared_ptr<Planet>> &planets)
{
	string baseName = "stage";
	string count = to_string(stage);

	ifstream file;
	const string stageFileName = "Resources/Stage/" + baseName + count + ".txt";
	file.open(stageFileName);

	if (file.fail())
	{
		file.close();
		return false;
	}

	string line;

	
	std::vector<MakePlanetData> datas;

	Vector3 setColor;
	XMFLOAT4 color;
	int ID = stage;
	bool isSpawn = false;
	PlanetType type = PlanetType::BASE;

	int pCount = 0;
	int sCount = 0;
	int maxCount = 0;

	Vector3 basePos;
	while (getline(file, line))
	{
		//','��' '�ɕϊ�
		replace(line.begin(), line.end(), ',', ' ');
		istringstream lineData(line);

		string key;
		getline(lineData, key, ' ');


		if (key == "n")
		{
		lineData >> maxCount;
		planets.resize(maxCount);
		for (auto &e : planets)
		{
			e = make_shared<Planet>();
		}
		datas.resize(maxCount);
		}

		//b (����W)
		if (key == "b")
		{
			//�O���̏���������o��
			lineData >> basePos.x;
			lineData >> basePos.y;
			lineData >> basePos.z;
		}


		//p (����W����̑��΍��W)
		if (key == "p" && pCount < maxCount)
		{
			//�O���̏���������o��
			lineData >> datas[pCount].pos.x;
			lineData >> datas[pCount].pos.y;
			lineData >> datas[pCount].pos.z;

			//����W�����Z���ă��[���h���W���v�Z
			datas[pCount].pos += basePos;
			//���̃C���f�b�N�X����i�߂�
			pCount++;
		}

		//s (�X�P�[��)
		if (key == "s" && sCount < maxCount)
		{
			//�X�P�[�����O�����󂯎��
			lineData >> datas[sCount].scale;
			//���̃C���f�b�N�X����i�߂�
			sCount++;
		}

		//c (�J���[)
		if (key == "c")
		{
			lineData >> setColor.x;
			lineData >> setColor.y;
			lineData >> setColor.z;


			setColor *= (1.0f/255.0f);
			color.x = setColor.x;
			color.y = setColor.y;
			color.z = setColor.z;
			color.w = 1.0f;
		}

		ID = stage;

		//d (�����`��t���O)
		if (key == "d")
		{
			lineData >> isSpawn;
		}

		type = PlanetType::BASE;
	}
	for (int i = 0; i < maxCount; i++)
	{
		planets[i]->Init(datas[i].pos, datas[i].scale, color, ID, isSpawn, type);
	}

	//�t�@�C���̓ǂݍ��݂��I������̂ŕ���
	file.close();
	return true;
}

bool LoadGateFile(int stage, std::vector<std::shared_ptr<Gate>> &gates)
{
	string baseName = "gate";
	string count = to_string(stage);

	ifstream file;
	const string stageFileName = "Resources/Stage/" + baseName + count + ".txt";
	//�t�@�C����ǂݍ���
	file.open(stageFileName);

	//�t�@�C���ǂݍ��݂Ɏ��s������false��Ԃ�
	if (file.fail())
	{
		file.close();
		return false;
	}
	string line;


	std::vector<MakeGateData> datas;
	std::vector<std::shared_ptr<Gate>>tmpGates;
	Vector3 setColor;
	XMFLOAT4 color;
	int ID = stage;
	bool isSpawn = false;
	PlanetType type = PlanetType::BASE;

	int pCount = 0;
	int aCount = 0;
	int maxCount = 0;

	Vector3 basePos;
	while (getline(file, line))
	{
		//','��' '�ɕϊ�
		replace(line.begin(), line.end(), ',', ' ');
		istringstream lineData(line);

		string key;
		getline(lineData, key, ' ');

		//n ��
		if (key == "n")
		{
			//�Q�[�g�̏������ݒ肷��
			lineData >> maxCount;
			//�����������
			tmpGates.resize(maxCount);
			for (auto &e : tmpGates)
			{
				e = make_unique<Gate>();
			}
			datas.resize(maxCount);
		}

		//b ����W
		if (key == "b")
		{
			//�O���̏���������o��
			lineData >> basePos.x;
			lineData >> basePos.y;
			lineData >> basePos.z;
		}


		//p(����W����̑��΍��W)
		if (key == "p" && pCount < maxCount)
		{
			//�O���̏���������o��
			lineData >> datas[pCount].pos.x;
			lineData >> datas[pCount].pos.y;
			lineData >> datas[pCount].pos.z;

			//����W�����Z���ă��[���h���W���v�Z
			datas[pCount].pos += basePos;
			//���̃C���f�b�N�X����i�߂�
			pCount++;
		}

		//a(�A���O��)
		if (key == "a" && aCount < maxCount)
		{
			//�O������������o��
			lineData >> datas[aCount].angle.x;
			lineData >> datas[aCount].angle.y;
			lineData >> datas[aCount].angle.z;
			//�����Ȃ̂Ő��K��
			datas[aCount].angle = datas[aCount].angle.normalize();
			//���̃C���f�b�N�X����i�߂�
			aCount++;
		}

		//c (�J���[)
		if (key == "c")
		{
			lineData >> setColor.x;
			lineData >> setColor.y;
			lineData >> setColor.z;


			setColor *= (1.0f / 255.0f);
			color.x = setColor.x;
			color.y = setColor.y;
			color.z = setColor.z;
			color.w = 1.0f;
		}
		ID = stage;

		//d(�����`��t���O)
		if (key == "d")
		{
			//0 : �`�悵�Ȃ�, 1 : �`�悷��
			lineData >> isSpawn;
		}
	}
	for (int i = 0; i < maxCount; i++)
	{
		tmpGates[i]->Init(datas[i].pos, datas[i].angle, ID, isSpawn);
		tmpGates[i]->SetColor(color);
		gates.emplace_back(tmpGates[i]);
	}

	//�t�@�C���̓ǂݍ��݂��I������̂ŕ���
	file.close();
	return true;
}

void SaveStageFile(const std::vector<std::weak_ptr<Planet>>& planets, const std::vector<std::weak_ptr<Gate>>& Gates, bool isStartDraw)
{
	//�f���ƃQ�[�g�̐F�f�[�^
	XMFLOAT4 color;
	{
		//
		if (planets.size() <= 0)
		{
			return;
		}

		std::ofstream stage;
		//�t�@�C����ǂݍ���
		string count = to_string(planets[0].lock()->GetID());

		const string stageFileName = "Resources/Stage/stage" + count + ".txt";

		stage.open(stageFileName, std::ios::out);


		//�f���̌��̃f�[�^
		string planetCounts = "n " + to_string(static_cast<int>(planets.size())) + "\n";
		//�f���̍��W�f�[�^
		string planetsPos;
		//�f���̃X�P�[���f�[�^
		string planetsScale;

		//�f���̐F�f�[�^
		XMVECTOR colorV = XMLoadFloat4(&planets[0].lock()->GetColor());
		colorV *= 255.0f;
		XMStoreFloat4(&color, colorV);
		int R, G, B;
		R = static_cast<int>(color.x);
		G = static_cast<int>(color.y);
		B = static_cast<int>(color.z);
		string planetsColor = "c " + to_string(R) + "," + to_string(G) + "," + to_string(B) + "\n";
		//�f���̏����`��f�[�^
		string planetsStartDraw;
		if (isStartDraw)
		{
			planetsStartDraw = "d 1";
		}
		else
		{
			planetsStartDraw = "d 0";
		}

		//���ׂĂ̘f���̍��W�ƃX�P�[���p��
		for (auto& e : planets)
		{
			Vector3 pos = e.lock()->GetPos();
			planetsPos += ("p " + to_string(pos.x) + " " + to_string(pos.y) + " " + to_string(pos.z) + "\n");

			if (e.lock()->GetIsSpawn())
			{
				planetsScale += ("s " + to_string(e.lock()->GetScale()) + "\n");
			}
			else
			{
				planetsScale += ("s " + to_string(e.lock()->GetStartScale()) + "\n");
			}
		}

		stage << planetCounts << endl << planetsPos << endl << planetsScale << endl << planetsColor << endl << planetsStartDraw;

		stage.close();
	}

	{
		//
		if (Gates.size() <= 0)
		{
			return;
		}
		std::ofstream gateFile;
		//�t�@�C����ǂݍ���
		string count = to_string(Gates[0].lock()->GetID());

		const string gateFileName = "Resources/Stage/gate" + count + ".txt";

		gateFile.open(gateFileName, std::ios::out);


		//�f���̌��̃f�[�^
		string gatesCounts = "n " + to_string(static_cast<int>(Gates.size())) + "\n";
		//�f���̍��W�f�[�^
		string gatesPos;
		//�f���̃X�P�[���f�[�^
		string gatesAngle;


		int R, G, B;
		R = static_cast<int>(color.x);
		G = static_cast<int>(color.y);
		B = static_cast<int>(color.z);
		string gatesColor = "c " + to_string(R) + "," + to_string(G) + "," + to_string(B) + "\n";
		//�f���̏����`��f�[�^
		string gatesStartDraw;
		if (isStartDraw)
		{
			gatesStartDraw = "d 1";
		}
		else
		{
			gatesStartDraw = "d 0";
		}

		//���ׂĂ̘f���̍��W�ƃX�P�[���p��
		for (auto& e : Gates)
		{
			Vector3 pos = e.lock()->GetPos();
			gatesPos += ("p " + to_string(pos.x) + " " + to_string(pos.y) + " " + to_string(pos.z) + "\n");

			Vector3 angle = e.lock()->GetAngle();
			gatesAngle += ("a " + to_string(angle.x) + " " + to_string(angle.y) + " " + to_string(angle.z) + "\n");
		}

		gateFile << gatesCounts << endl << gatesPos << endl << gatesAngle << endl << gatesColor << endl << gatesStartDraw;

		gateFile.close();
	}
}

void SaveAllStageFile(const std::list<std::shared_ptr<Planet>>& planets, const std::vector<std::shared_ptr<Gate>>& Gates)
{
	int stageNum = 0;

	//�擪�̘f���͒��S�̂��̂�����X�L�b�v����t���O
	bool isNotFirst = false;
	//�X�e�[�W���Ȃ��Ȃ�܂ŃX�e�[�W���Ԃ��
	while (true)
	{
		std::vector<std::weak_ptr<Planet>> tmpPlanets;
		std::vector<std::weak_ptr<Gate>> tmpGates;
		for (auto& e : planets)
		{

			if (isNotFirst && e->GetID() == stageNum)
			{
				tmpPlanets.emplace_back(e);
			}
			else
			{
				isNotFirst = true;
			}
		}
		for (auto& e : Gates)
		{
			if (e->GetID() == stageNum)
			{
				tmpGates.emplace_back(e);
			}
		}

		//�f�������邩�ǂ���
		if (static_cast<int>(tmpPlanets.size()) > 0)
		{//��������ۑ�����
			if (stageNum > 0)
			{//�X�e�[�W1�ȍ~�͏�����Ԃ��ƌ����Ȃ�
				SaveStageFile(tmpPlanets, tmpGates, false);
			}
			else
			{//�X�e�[�W0�͏����z�u�ł��łɌ����Ă������
				SaveStageFile(tmpPlanets, tmpGates, true);
			}
			//�X�e�[�W�ԍ������Z
			stageNum++;
		}
		else
		{//�Ȃ�������I��
			break;
		}
	}
}

