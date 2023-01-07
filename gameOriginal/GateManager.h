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

	//�Q�[�g�p�[�e�B�N���̃I�u�W�F�N�g���󂯎��
	void SetGateParticle(GateParticle* gateParticle);

	//�v���C���[�̃X�e�[�^�X���󂯎��
	void ReceivePlayerStatus(const GameDatas::PlayerStatus& playerStatus);

	void GateSpawn(int stageNum);
	//���݂̃Q�[�g�f�[�^��n��
	const std::vector<std::shared_ptr<Gate>>& GetGates();
public://�G�f�B�^�p

	/// <summary>
	/// �Q�[�g���쐬����
	/// </summary>
	/// <param name="controllGate">�R�s�[���̃I�u�W�F�N�g</param>
	/// <returns></returns>
	std::weak_ptr<Gate> MakeGate(std::weak_ptr<Gate>& controllGate, int stageNum);

	/// <summary>
	/// �S�Q�[�g����ҏW�������ԍ��̃X�e�[�W�����W����
	/// </summary>
	/// <param name="editStageNum">�ҏW����X�e�[�W�ԍ�</param>
	/// <param name="makeGates">���W�����X�e�[�W������z��</param>
	/// <param name="controllGate">����ΏۂɂƂ�f��</param>
	/// <returns></returns>
	bool SetMakeGates(int &editStageNum, std::vector<std::weak_ptr<Gate>>& makeGates, std::weak_ptr<Gate>& controllGate);
private:
	//�Q�[�����ɔz�u����Ă���Q�[�g
	std::vector<std::shared_ptr<Gate>> gates_;

	//�Q�[�g�ʉߎ��ɔ�������p�[�e�B�N��
	GateParticle* gateParticles_ = nullptr;

	//���݂̃v���C���[�̏��
	GameDatas::PlayerStatus playerStatus_ = GameDatas::PlayerStatus::STAND;
};