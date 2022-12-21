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

	//�Q�[�g�p�[�e�B�N���̃I�u�W�F�N�g���󂯎��
	void SetGateParticle(GateParticle* gateParticle);
private:
	std::vector<std::unique_ptr<Gate>> gates_;

	//�Q�[�g�ʉߎ��ɔ�������p�[�e�B�N��
	GateParticle* gateParticles_;
};