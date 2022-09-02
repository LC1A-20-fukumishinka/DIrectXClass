#include "LoadPlanet.h"
#include "Vector3.h"
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;
using namespace DirectX;
bool LoadStageFile(int stage, std::shared_ptr<Planet> &planet)
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

	planet = make_shared<Planet>();

	Vector3 pos;
	float size = 0.0f;
	XMFLOAT4 color;
	int ID = stage;
	bool isSpawn = false;
	PlanetType type = PlanetType::BASE;

	while (getline(file, line))
	{
		//','‚ð' '‚É•ÏŠ·
		replace(line.begin(), line.end(), ',', ' ');
		istringstream line_Data(line);

		string key;
		getline(line_Data, key, ' ');

		//p
		if (key == "p")
		{
			line_Data >> pos.x;
			line_Data >> pos.y;
			line_Data >> pos.z;
		}

		//s
		if (key == "s")
		{
			line_Data >> size;

		}

		//c
		if (key == "c")
		{
			line_Data >> color.x;
			line_Data >> color.y;
			line_Data >> color.z;
			line_Data >> color.w;

		}

		ID = stage;

		//b
		if (key == "b")
		{
			line_Data >> isSpawn;
		}

		type = PlanetType::BASE;
	}
	planet->Init(pos, size, color, ID, isSpawn, type);

	return true;
}
