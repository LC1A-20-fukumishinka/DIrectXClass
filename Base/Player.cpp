#include "Player.h"
#include "DirectInput.h"
#include "DebugText.h"
#include "SphereCollider.h"
#include "TextureMgr.h"
using namespace DirectX;
Player *Player::Create(Camera *camera, Light *light, Object3D *parent)
{
	Player *instance = new Player();
	if (instance == nullptr)
	{
		return nullptr;
	}

	instance->Init();
	return instance;
}

Player::~Player()
{
	delete hitPart;
}

void Player::Init()
{
	spriteTex = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/texture.png");

	Object3D::Init();

	//コライダーの追加
	float radius = 0.6f;
	//半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0, 0 }), radius));
	hitPart = ParticleManager::Create();
}

void Player::Update()
{
	Input *input = Input::Instance();

	//ATrigger,Dで旋回
	if (input->Key(DIK_A))
	{
		rotation.y -= 0.02f;
	}
	else if (input->Key(DIK_D))
	{
		rotation.y += 0.02f;
	}

	//移動ベクトルをY軸周りの角度で回転
	XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationY(rotation.y);
	move = XMVector3TransformNormal(move, matRot);

	//向いている方向に移動
	if (input->Key(DIK_S))
	{
		position.x -= move.m128_f32[0];
		position.y -= move.m128_f32[1];
		position.z -= move.m128_f32[2];
	}
	else if (input->Key(DIK_W))
	{
		position.x += move.m128_f32[0];
		position.y += move.m128_f32[1];
		position.z += move.m128_f32[2];
	}
	//行列の更新
	Object3D::Update();
	hitPart->Update();

}

void Player::OnCollision(const CollisionInfo &info)
{

	for (int i = 0; i < 1; ++i)
	{
		const float rnd_vel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		hitPart->Add(10, XMFLOAT3(info.inter.m128_f32), vel, XMFLOAT3(), 0.0f, 1.0f);
	}
}

void Player::modelDraw(PipeClass::PipelineSet *pipelineSet, bool isSetTexture, int textureNumber)
{
	Object3D::modelDraw(pipelineSet);
	hitPart->Draw(spriteTex);
}

void Player::SetCamera(Camera *camera)
{
	Object3D::SetCamera(camera);
	hitPart->SetCamera(camera);
}
