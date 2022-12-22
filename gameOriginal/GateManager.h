#include "Gate.h"
#include <vector>
#include <memory>
#include "gameConstData.h"
class GateParticle;
class GateManager
{
public:
	GateManager();
	~GateManager();

	void Init();
	void Update();
	void Draw();
	void Finalize();
	void Reset();
	bool LoadStage(int stage);

	bool Collision(const Vector3 &pos, float speed);

	//ゲートパーティクルのオブジェクトを受け取る
	void SetGateParticle(GateParticle* gateParticle);

	//プレイヤーのステータスを受け取る
	void ReceivePlayerStatus(const GameDatas::PlayerStatus &playerStatus);
private:
	//ゲーム内に配置されているゲート
	std::vector<std::unique_ptr<Gate>> gates_;

	//ゲート通過時に発生するパーティクル
	GateParticle* gateParticles_ = nullptr;

	//現在のプレイヤーの状態
	GameDatas::PlayerStatus playerStatus_ = GameDatas::PlayerStatus::STAND;
};