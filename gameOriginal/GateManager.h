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

	bool Collision(const Vector3& pos, float speed);

	//ゲートパーティクルのオブジェクトを受け取る
	void SetGateParticle(GateParticle* gateParticle);

	//プレイヤーのステータスを受け取る
	void ReceivePlayerStatus(const GameDatas::PlayerStatus& playerStatus);

	void GateSpawn(int stageNum);
	//現在のゲートデータを渡す
	const std::vector<std::shared_ptr<Gate>>& GetGates();
public://エディタ用

	/// <summary>
	/// ゲートを作成する
	/// </summary>
	/// <param name="controllGate">コピー元のオブジェクト</param>
	/// <returns></returns>
	std::weak_ptr<Gate> MakeGate(std::weak_ptr<Gate>& controllGate, int stageNum);

	/// <summary>
	/// 全ゲートから編集したい番号のステージを収集する
	/// </summary>
	/// <param name="editStageNum">編集するステージ番号</param>
	/// <param name="makeGates">収集したステージを入れる配列</param>
	/// <param name="controllGate">操作対象にとる惑星</param>
	/// <returns></returns>
	bool SetMakeGates(int &editStageNum, std::vector<std::weak_ptr<Gate>>& makeGates, std::weak_ptr<Gate>& controllGate);
private:
	//ゲーム内に配置されているゲート
	std::vector<std::shared_ptr<Gate>> gates_;

	//ゲート通過時に発生するパーティクル
	GateParticle* gateParticles_ = nullptr;

	//現在のプレイヤーの状態
	GameDatas::PlayerStatus playerStatus_ = GameDatas::PlayerStatus::STAND;
};