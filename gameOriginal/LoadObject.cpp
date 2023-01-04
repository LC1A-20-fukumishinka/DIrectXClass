#include "LoadObject.h"
#include "Vector3.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <json.hpp>
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
			//�f���̏������ݒ肷��
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
		gates.emplace_back(tmpGates[i]);
	}

	//�t�@�C���̓ǂݍ��݂��I������̂ŕ���
	file.close();
	return true;
}

void SaveStageFile(int stage, std::vector<std::shared_ptr<Planet>>& planets)
{
}

