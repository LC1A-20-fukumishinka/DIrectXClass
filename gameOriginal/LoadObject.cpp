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
		//','を' 'に変換
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

		//b (基準座標)
		if (key == "b")
		{
			//三軸の情報を一つずつ取り出す
			lineData >> basePos.x;
			lineData >> basePos.y;
			lineData >> basePos.z;
		}


		//p (基準座標からの相対座標)
		if (key == "p" && pCount < maxCount)
		{
			//三軸の情報を一つずつ取り出す
			lineData >> datas[pCount].pos.x;
			lineData >> datas[pCount].pos.y;
			lineData >> datas[pCount].pos.z;

			//基準座標を加算してワールド座標を計算
			datas[pCount].pos += basePos;
			//情報のインデックスを一つ進める
			pCount++;
		}

		//s (スケール)
		if (key == "s" && sCount < maxCount)
		{
			//スケーリング情報を受け取る
			lineData >> datas[sCount].scale;
			//情報のインデックスを一つ進める
			sCount++;
		}

		//c (カラー)
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

		//d (初期描画フラグ)
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

	//ファイルの読み込みが終わったので閉じる
	file.close();
	return true;
}

bool LoadGateFile(int stage, std::vector<std::shared_ptr<Gate>> &gates)
{
	string baseName = "gate";
	string count = to_string(stage);

	ifstream file;
	const string stageFileName = "Resources/Stage/" + baseName + count + ".txt";
	//ファイルを読み込む
	file.open(stageFileName);

	//ファイル読み込みに失敗したらfalseを返す
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
		//','を' 'に変換
		replace(line.begin(), line.end(), ',', ' ');
		istringstream lineData(line);

		string key;
		getline(lineData, key, ' ');

		//n 個数
		if (key == "n")
		{
			//惑星の上限個数を設定する
			lineData >> maxCount;
			//上限個数分生成
			tmpGates.resize(maxCount);
			for (auto &e : tmpGates)
			{
				e = make_unique<Gate>();
			}
			datas.resize(maxCount);
		}

		//b 基準座標
		if (key == "b")
		{
			//三軸の情報を一つずつ取り出す
			lineData >> basePos.x;
			lineData >> basePos.y;
			lineData >> basePos.z;
		}


		//p(基準座標からの相対座標)
		if (key == "p" && pCount < maxCount)
		{
			//三軸の情報を一つずつ取り出す
			lineData >> datas[pCount].pos.x;
			lineData >> datas[pCount].pos.y;
			lineData >> datas[pCount].pos.z;

			//基準座標を加算してワールド座標を計算
			datas[pCount].pos += basePos;
			//情報のインデックスを一つ進める
			pCount++;
		}

		//a(アングル)
		if (key == "a" && aCount < maxCount)
		{
			//三軸情報を一つずつ取り出す
			lineData >> datas[aCount].angle.x;
			lineData >> datas[aCount].angle.y;
			lineData >> datas[aCount].angle.z;
			//向きなので正規化
			datas[aCount].angle = datas[aCount].angle.normalize();
			//情報のインデックスを一つ進める
			aCount++;
		}
		ID = stage;

		//d(初期描画フラグ)
		if (key == "d")
		{
			//0 : 描画しない, 1 : 描画する
			lineData >> isSpawn;
		}
	}
	for (int i = 0; i < maxCount; i++)
	{
		tmpGates[i]->Init(datas[i].pos, datas[i].angle, ID, isSpawn);
		gates.emplace_back(tmpGates[i]);
	}

	//ファイルの読み込みが終わったので閉じる
	file.close();
	return true;
}

void SaveStageFile(int stage, std::vector<std::shared_ptr<Planet>>& planets)
{
}

