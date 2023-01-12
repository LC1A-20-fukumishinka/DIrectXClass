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
		//','を' 'に変換
		replace(line.begin(), line.end(), ',', ' ');
		istringstream lineData(line);

		string key;
		getline(lineData, key, ' ');

		//n 個数
		if (key == "n")
		{
			//ゲートの上限個数を設定する
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

		//c (カラー)
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
		tmpGates[i]->SetColor(color);
		gates.emplace_back(tmpGates[i]);
	}

	//ファイルの読み込みが終わったので閉じる
	file.close();
	return true;
}

void SaveStageFile(const std::vector<std::weak_ptr<Planet>>& planets, const std::vector<std::weak_ptr<Gate>>& Gates, bool isStartDraw)
{
	//惑星とゲートの色データ
	XMFLOAT4 color;
	{
		//
		if (planets.size() <= 0)
		{
			return;
		}

		std::ofstream stage;
		//ファイルを読み込む
		string count = to_string(planets[0].lock()->GetID());

		const string stageFileName = "Resources/Stage/stage" + count + ".txt";

		stage.open(stageFileName, std::ios::out);


		//惑星の個数のデータ
		string planetCounts = "n " + to_string(static_cast<int>(planets.size())) + "\n";
		//惑星の座標データ
		string planetsPos;
		//惑星のスケールデータ
		string planetsScale;

		//惑星の色データ
		XMVECTOR colorV = XMLoadFloat4(&planets[0].lock()->GetColor());
		colorV *= 255.0f;
		XMStoreFloat4(&color, colorV);
		int R, G, B;
		R = static_cast<int>(color.x);
		G = static_cast<int>(color.y);
		B = static_cast<int>(color.z);
		string planetsColor = "c " + to_string(R) + "," + to_string(G) + "," + to_string(B) + "\n";
		//惑星の初期描画データ
		string planetsStartDraw;
		if (isStartDraw)
		{
			planetsStartDraw = "d 1";
		}
		else
		{
			planetsStartDraw = "d 0";
		}

		//すべての惑星の座標とスケール用意
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
		//ファイルを読み込む
		string count = to_string(Gates[0].lock()->GetID());

		const string gateFileName = "Resources/Stage/gate" + count + ".txt";

		gateFile.open(gateFileName, std::ios::out);


		//惑星の個数のデータ
		string gatesCounts = "n " + to_string(static_cast<int>(Gates.size())) + "\n";
		//惑星の座標データ
		string gatesPos;
		//惑星のスケールデータ
		string gatesAngle;


		int R, G, B;
		R = static_cast<int>(color.x);
		G = static_cast<int>(color.y);
		B = static_cast<int>(color.z);
		string gatesColor = "c " + to_string(R) + "," + to_string(G) + "," + to_string(B) + "\n";
		//惑星の初期描画データ
		string gatesStartDraw;
		if (isStartDraw)
		{
			gatesStartDraw = "d 1";
		}
		else
		{
			gatesStartDraw = "d 0";
		}

		//すべての惑星の座標とスケール用意
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

	//先頭の惑星は中心のものだからスキップするフラグ
	bool isNotFirst = false;
	//ステージがなくなるまでステージをぶん回す
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

		//惑星があるかどうか
		if (static_cast<int>(tmpPlanets.size()) > 0)
		{//あったら保存する
			if (stageNum > 0)
			{//ステージ1以降は初期状態だと見えない
				SaveStageFile(tmpPlanets, tmpGates, false);
			}
			else
			{//ステージ0は初期配置ですでに見えているもの
				SaveStageFile(tmpPlanets, tmpGates, true);
			}
			//ステージ番号を加算
			stageNum++;
		}
		else
		{//なかったら終了
			break;
		}
	}
}

