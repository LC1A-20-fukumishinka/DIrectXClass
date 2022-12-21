#include "Gate.h"
#include <vector>
#include <memory>

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
private:
	std::vector<std::unique_ptr<Gate>> gates_;

	//ゲート通過時に発生するパーティクル
	GateParticle* gateParticles_;
};