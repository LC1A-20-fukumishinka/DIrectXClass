#include "LoadPlanet.h"
#include "Vector3.h"
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;
using namespace DirectX;


struct MakePlanetData
{
	Vector3 pos;
	float scale = 0;
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
		//','‚ð' '‚É•ÏŠ·
		replace(line.begin(), line.end(), ',', ' ');
		istringstream line_Data(line);

		string key;
		getline(line_Data, key, ' ');


		if (key == "n")
		{
		line_Data >> maxCount;
		planets.resize(maxCount);
		for (auto &e : planets)
		{
			e = make_shared<Planet>();
		}
		datas.resize(maxCount);
		}

		if (key == "b")
		{
			line_Data >> basePos.x;
			line_Data >> basePos.y;
			line_Data >> basePos.z;
		}


		//p
		if (key == "p" && pCount < maxCount)
		{
			line_Data >> datas[pCount].pos.x;
			line_Data >> datas[pCount].pos.y;
			line_Data >> datas[pCount].pos.z;
			datas[pCount].pos += basePos;
			pCount++;
		}

		//s
		if (key == "s" && sCount < maxCount)
		{
			line_Data >> datas[sCount].scale;
			sCount++;
		}

		//c
		if (key == "c")
		{
			line_Data >> setColor.x;
			line_Data >> setColor.y;
			line_Data >> setColor.z;


			setColor *= (1.0f/255.0f);
			color.x = setColor.x;
			color.y = setColor.y;
			color.z = setColor.z;
			color.w = 1.0f;
		}

		ID = stage;

		//b
		if (key == "d")
		{
			line_Data >> isSpawn;
		}

		type = PlanetType::BASE;
	}
	for (int i = 0; i < maxCount; i++)
	{
		planets[i]->Init(datas[i].pos, datas[i].scale, color, ID, isSpawn, type);
	}

	return true;
}
