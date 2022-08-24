#pragma once
#include "Sprite.h"
#include "Object3D.h"
#include "Model.h"
#include "IScene.h"
#include "../Light.h"
#include "SphereCollider.h"
#include "../Collision/CollisionMgr.h"
#include "particleManager.h"
#include "../LightGroup.h"
#include "../PointLight.h"
#include "../SpotLight.h"
#include "FbxModel.h"
#include "FbxObject3D.h"
#include "../gameOriginal/Planet.h"
#include "../gameOriginal/GravityPlayer.h"
#include "../gameOriginal/GameCamera.h"
#include "../ShadowCamera.h"
#include "MultiRenderTarget.h"
#include "PostEffect.h"
#include "PostBasePipeline.h"
#include "../gameOriginal/Flag.h"
#include "../gameOriginal/Block.h"
#include "../gameOriginal/TitlePostEffect.h"
#include "Sprite.h"
#include "../Box3D.h"
#include "../InformationBoard.h"
class TouchableObject;
class GameScene : public IScene
{
public:
	void Init() override;
	void Update() override;
	void PreDraw() override;
	void MainDraw() override;
	void Finalize() override;
private:
	//���X�^�[�g�֐�
	void Restart();

	//����ɕt�����邻�ꂼ��̃��Z�b�g�֐�
	void lightsRestart();
	void ObjectRestart();
	void MovePlanet();
	void ImguiUpdate();

private:

	enum ClearAnimationStatus
	{
		STANDBY,
		STOP,
		MOVECAM,
		SPAWNPLANET,
		RETURNCAM,
	};
	/// <summary>
	/// �f���Q�o���A�j���[�V�����S�̃t���[�i�Ƃ肠�����������ƍ��֐����j
	/// </summary>
	void AnimationTestUpdate();
private:

	std::shared_ptr<Planet> star;

	//�����I�ɒǉ�
	std::unique_ptr<MultiRenderTarget> shadowRenderTarget_;
	std::unique_ptr<MultiRenderTarget> GBTarget_;
	std::unique_ptr<MultiRenderTarget> MonoTarget_;
	std::unique_ptr<MultiRenderTarget> MosaicTarget_;
	std::unique_ptr<MultiRenderTarget> StartTarget_;
	std::unique_ptr<GameCamera> cam_;
	std::unique_ptr<ShadowCamera> shadowCam_;
	std::unique_ptr<Light> light_;
	std::unique_ptr<PointLight> pointLight_;
	std::unique_ptr<SpotLight> spotLight_;
	std::unique_ptr<LightGroup> lightGroup_;

	std::unique_ptr<Model> groundModel_;
	std::unique_ptr<Model> playerModel_;
	std::unique_ptr<Model> domeModel_;

	std::unique_ptr<Object3D> objDome_;
	std::unique_ptr<Object3D> objGround_;
	std::unique_ptr<FbxModel> templeModel_;
	std::unique_ptr<FbxObject3D> temple_;
	std::unique_ptr<GravityPlayer> player_;
	std::unique_ptr<PostEffect> postTest_;
	TitlePostEffect titlePostEffect_;
	std::unique_ptr<Sprite> clearText_;
	PostBasePipeline NormalDrawPipeline_;

	std::list<Flag> testFlag_;
	std::list<Block> testBlock_;


	InformationBoard testBoard_;
	Sprite Dark_;
	//BoxObj3D box;
	bool isPause_ = false;
	bool isGB_ = false;
	bool isMono_ = false;
	bool isMosaic_ = false;
	int DrawTexture_ = -1;

	ClearAnimationStatus clearStatus_ = STANDBY;
private:
	void MakeFlag(std::weak_ptr<Planet> base, Vector3 angle, float scale);

	int GetFlagCount();

	bool isGameClear_ = false;
	bool isGameTitle_ = true;
};