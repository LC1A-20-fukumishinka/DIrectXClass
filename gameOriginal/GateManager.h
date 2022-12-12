#include "Gate.h"
#include <vector>
#include <memory>
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
private:
	std::vector<std::unique_ptr<Gate>> gates_;
};