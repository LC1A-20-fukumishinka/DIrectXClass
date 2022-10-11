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
#include "../gameOriginal/InformationBoard.h"
#include "../gameOriginal/GuidingStar.h"
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

	void IngameUpdate();
	void TitleUpdate();
	//リスタート関数
	void Restart();

	//それに付随するそれぞれのリセット関数
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
	/// 惑星群出現アニメーション全体フロー（とりあえずさっさと作れ関数化）
	/// </summary>
	void AnimationTestUpdate();
private:

	std::shared_ptr<Planet> star;

	//実験的に追加
	std::unique_ptr<MultiRenderTarget> shadowRenderTarget_;
	std::unique_ptr<MultiRenderTarget> GBTarget_;
	std::unique_ptr<MultiRenderTarget> MonoTarget_;
	std::unique_ptr<MultiRenderTarget> MosaicTarget_;
	std::unique_ptr<MultiRenderTarget> StartTarget_;
	std::unique_ptr<MultiRenderTarget> TitleTarget_;
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


	std::unique_ptr<Sprite> pressStartText_;
	Easing pressStartTextAnimationEase_;
	bool isTitleAnimationFlip = false;
	std::list<Flag> testFlag_;
	std::list<Block> testBlock_;


	std::list<InformationBoard> testBoards_;
	Sprite Dark_;
	//BoxObj3D box;
	bool isPause_ = false;
	bool isGB_ = false;
	bool isMono_ = false;
	bool isMosaic_ = false;
	int DrawTexture_ = -1;
	int stageNum = 0;
	ClearAnimationStatus clearStatus_ = STANDBY;

private://ガイド流れ星管理
	std::vector<GuidingStar> testStar;

	void makeGuide();
private://FlagManager用
	void MakeFlag(std::weak_ptr<Planet> base, Vector3 angle, float scale);

	int GetFlagCount();

	bool isGameClear_ = false;
	bool isGameTitle_ = true;
};