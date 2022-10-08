#pragma once
#include "Planet.h"
#include <list>
#include <memory>
class PlanetManager
{
private:
	PlanetManager();
public:
	~PlanetManager();

	PlanetManager(const PlanetManager &obj) = delete;
	PlanetManager &operator=(const PlanetManager &obj) = delete;

	static std::unique_ptr<PlanetManager> &Instance();

	void Init();
	void Update();
	void Draw();
	void Finalize();
public:
	bool GetGrabPlanet(std::shared_ptr<Planet> &planet, const DirectX::XMFLOAT3 &pos, const DirectX::XMFLOAT3 &angle);


	bool MovePlanet(std::shared_ptr<Planet> &planet, const DirectX::XMFLOAT3 &pos);

	/// <summary>
	/// 指定した番号の惑星を返す関数（とりあえずで作った関数、テスト用）
	/// </summary>
	/// <param name="getPlanetNum">番号</param>
	/// <returns>指定された惑星（存在しない番号の場合は先頭の番号を返す）</returns>
	std::weak_ptr<Planet> GetPlanet(int getPlanetNum);

	void AddPlanet(const DirectX::XMFLOAT3 &pos, float size, const DirectX::XMFLOAT4 &color, int ID,  bool isSpawn = true, const PlanetType &type = BRIDGE);

	void Reset();

	/// <summary>
	/// 惑星の持つIDを参照して出現させる
	/// </summary>
	/// <param name="ID">出現させる</param>
	void IDSpawn(int ID);
	/// <summary>
	/// 現在いない状態になっている惑星を出現させる
	/// </summary>
	void AllSpawn();

	/// <summary>
	/// アニメーションが終了した瞬間のみtrueを返す
	/// </summary>
	/// <returns>アニメーションが終了したフラグ</returns>
	bool SpawnAnimationEnd(int ID);
public:
	void SetCamera(Camera *cam);
	void SetShadowCamera(Camera *cam);
	void SetShadowTexture(int shadowTextureNum);

	bool LoadStage(int stage);

	/// <summary>
	/// 指定の番号のbase属性の惑星のポインタを取得する
	/// </summary>
	/// <param name="stageNum">番号の指定（ない場合や範囲外を指定している場合一番後ろの惑星を返す）</param>
	/// <returns>惑星のポインタ</returns>
	std::shared_ptr<Planet> GetBasePlanet(int stageNum = -1);
private:
	std::list<std::shared_ptr<Planet>> planets;

	bool isSpawnAnimation_ = false;

	static bool isMakeInstance;

};

