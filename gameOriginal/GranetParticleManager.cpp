#include "GranetParticleManager.h"
#include "TextureMgr.h"
GranetParticleManager::GranetParticleManager()
{
}

GranetParticleManager::~GranetParticleManager()
{
}

void GranetParticleManager::Init(Camera* camera)
{
	int effectTextureHandle = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/effect.png");

	fieldParticles_.Init();
	fieldParticles_.SetCamera(camera);
	fieldParticles_.SetTexture(effectTextureHandle);
	planetParticles_.Init();
	planetParticles_.SetCamera(camera);
	planetParticles_.SetTexture(effectTextureHandle);

	gateParticles_.Init();
	gateParticles_.SetCamera(camera);
	gateParticles_.SetTexture(effectTextureHandle);

	playerRandingParticles_.Init();
	playerRandingParticles_.SetCamera(camera);
	playerRandingParticles_.SetTexture(effectTextureHandle);
}

void GranetParticleManager::Update()
{
	fieldParticles_.SetPlayerDatas(makeData.cameraPos, makeData.cameraRot, makeData.playerGravity);
	gateParticles_.SetPlayerData(makeData.playerPos, makeData.playerMoveVec);
	fieldParticles_.Update();
	planetParticles_.Update();
	gateParticles_.Update();
	playerRandingParticles_.Update();
}

void GranetParticleManager::Draw()
{
	fieldParticles_.Draw();
	planetParticles_.Draw();
	gateParticles_.Draw();
	playerRandingParticles_.Draw();
}

void GranetParticleManager::SetMakeParticleDatas(const MakeParticleDatas& data)
{
	makeData = data;
}

PlanetParticle* GranetParticleManager::GetPlanetParticles()
{
	return &planetParticles_;
}

GateParticle* GranetParticleManager::GetGateParticles()
{
	return &gateParticles_;
}

PlayerRandingParticle* GranetParticleManager::GetPlayerRandingParticles()
{
	return &playerRandingParticles_;
}
