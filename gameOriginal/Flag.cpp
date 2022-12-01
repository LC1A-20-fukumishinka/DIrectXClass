#include "Flag.h"
#include "ModelPhongPipeline.h"
#include "Model.h"
#include "Planet.h"
#include "../ShadowPipeline.h"
#include "../Collision/Collision.h"
#include "TextureMgr.h"
#include "FukuMath.h"
#include <algorithm>
using namespace FukuMath;
using namespace DirectX;
std::unique_ptr<Model> Flag::sFlagModel;
std::unique_ptr<Model> Flag::sLightPillarModel;
std::unique_ptr<PipeClass::PipelineSet> Flag::sLightPipeline;
Camera *Flag::sShadowCamera = nullptr;

Camera *Flag::sMainCamera = nullptr;
LightGroup *Flag::sLights = nullptr;
int Flag::testTex = -1;
Flag::Flag()
{
	if (!sFlagModel)
	{
		sFlagModel = std::make_unique<Model>();
		sFlagModel->CreateModel("Flag");
	}

	if (!sLightPillarModel)
	{
		sLightPillarModel = std::make_unique<Model>();
		sLightPillarModel->CreateModel("InformationBoard");
		testTex = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/white1x1.png");
	}

	if (!sLightPipeline)
	{
		// 頂点レイアウト
		D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ // xy座標(1行で書いたほうが見やすい)
				"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // 法線ベクトル(1行で書いたほうが見やすい)
				"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // uv座標(1行で書いたほうが見やすい)
				"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
		};

		// デスクリプタレンジ
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
		descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

		// ルートパラメータ
		CD3DX12_ROOT_PARAMETER rootparams[4];
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);

		PipelineDesc desc;

		desc.VSname = L"Resources/shaders/LightPillarVS.hlsl";
		desc.PSname = L"Resources/shaders/LightPillarPS.hlsl";
		desc.inputLayout = inputLayout;
		desc.inputLayoutCount = _countof(inputLayout);
		desc.rootparams = rootparams;
		desc.rootparamsCount = _countof(rootparams);
		desc.blendName = GraphicsPipelineTypeName::BlendName::ALPHA;
		sLightPipeline = BaseGraphicsPipeline::CreatePipeLine(desc);
	}

	HRESULT result = S_FALSE;
	MyDirectX *myD = MyDirectX::Instance();

	result = myD->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(float) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&lightConstBuff2)
	);

	if (result)
	{
		assert(0);
	}
}

Flag::~Flag()
{
}

void Flag::Init(std::weak_ptr<Planet> basePlanet, const Vector3 &angle, float scale)
{
	upVec_ = angle;//オブジェクトの方向を設定
	worldScale_ = scale;
	upVec_ = upVec_.normalize();//一応正規化
	frontVec_ = Vector3(upVec_.y, -upVec_.x, 0.0f).cross(upVec_);
	basePlanet_ = basePlanet;
	object_.Init();
	lightPillarObject_.Init();
	object_.SetCamera(sMainCamera);
	object_.SetLightGroup(sLights);

	lightPillarObject_.SetLightGroup(sLights);
	lightPillarObject_.SetCamera(sMainCamera);
	RotationUpdate();

	if (!basePlanet_.expired())
	{
		//惑星の中心から
		Vector3 pos = basePlanet_.lock()->GetPos();
		//指定した方向(upVec)に惑星の半径分移動させた場所が
		pos += (upVec_ * basePlanet_.lock()->GetStartScale());
		object_.SetRotationVector(XMLoadFloat3(&frontVec_), XMLoadFloat3(&upVec_));

		//座標になる
		object_.SetPosition(pos);
		Vector3 identity = { 1.0f,1.0f ,1.0f };
		object_.SetScale(identity * (worldScale_));

		object_.SetModel(sFlagModel.get());

		lightPillarObject_.SetModel(sLightPillarModel.get());

		float pillarHeight = 15.0f;

		Vector3 pillarPos = pos + (upVec_ * (pillarHeight * 2.0f - 0.5f));
		lightPillarObject_.SetPosition(pillarPos);

		lightPillarObject_.SetScale(Vector3(1.0f, pillarHeight, 1.0f) * (worldScale_));

	}
}

void Flag::Update()
{
	object_.SetCamera(sMainCamera);
	object_.SetLightGroup(sLights);
	object_.SetRotationVector(XMLoadFloat3(&frontVec_), XMLoadFloat3(&upVec_));

	RotationUpdate();
}

void Flag::DrawShadow()
{
	//shadowObject_.Update();
	//shadowObject_.modelDraw(ShadowPipeline::Instance()->GetPipeLine());
}

void Flag::Finalize()
{
}

void Flag::Draw()
{
	if (GetIsDraw())
	{
		object_.Update();
		object_.modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
	}
	if (basePlanet_.lock()->GetIsSpawn() && lightRate_ > 0.0f)
	{/*
		LightPillarUpdate();
		lightPillarObject_.Update();
		lightPillarObject_.SetDrawBuffers(sLightPipeline.get(), true, testTex);
		MyDirectX::Instance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, lightConstBuff2->GetGPUVirtualAddress());
		lightPillarObject_.DrawCommand();*/
	}
}

void Flag::SetShadowCamera(Camera *camera)
{
	sShadowCamera = camera;
}

bool Flag::CollisionPlayer(float radius, const Vector3 &pos)
{

	if (!isDraw_) return false;

	Sphere playerSphere, flagSphere;
	playerSphere.center = XMLoadFloat3(&pos);
	playerSphere.radius = radius;
	flagSphere.center = XMLoadFloat3(&object_.GetWorldPos());
	flagSphere.radius = worldScale_;

	bool isCollision = Collision::CheckSphere2Sphere(playerSphere, flagSphere);

	if (isCollision)
	{
		//消える処理
		isDraw_ = false;
	}
	return isCollision;
}

bool Flag::GetIsDraw()
{
	return isDraw_ && basePlanet_.lock()->GetIsSpawn();
}

void Flag::Reset()
{
	isDraw_ = true;
}

void Flag::SetCamera(Camera *camera)
{
	sMainCamera = camera;
}


void Flag::SetLights(LightGroup *Lights)
{
	sLights = Lights;
}

void Flag::RotationUpdate()
{
	//XMVECTORに変換
	XMVECTOR frontVecV = XMLoadFloat3(&frontVec_);
	XMVECTOR upVecV = XMLoadFloat3(&upVec_);
	
	//上と正面が一致したら
	if (XMVector3Equal(frontVecV, upVecV))
	{//正面の向きを修正
		frontVec_ = Vector3(0.0f, -1.0f, 0.0f);
		frontVecV = XMLoadFloat3(&frontVec_);
	}


	object_.SetRotationVector(frontVecV, upVecV);
}

void Flag::LightPillarUpdate()
{

	if (GetIsDraw())
	{
		lightRate_ += 0.03f;
	}
	else
	{
		lightRate_ -= 0.03f;
	}

	lightRate_ = std::clamp(lightRate_, 0.0f, 1.0f);

	float easeRate = 0.0f;
	if (lightRate_ >= 1.0f)
	{
		lightAnimationRate_ += 0.01f;

		if (lightAnimationRate_ >= 1.0f)
		{
			lightAnimationRate_ = 0.0f;
			lightAnimationFlag_ = !lightAnimationFlag_;
		}
		if (lightAnimationFlag_)
		{
			easeRate = lightAnimationRate_;
		}
		else
		{
			easeRate = (1.0f - lightAnimationRate_);
		}
	}

	float tmpRate = lightRate_ + Easing::easeInOutCubic(easeRate);
	lightPillarObject_.SetLightGroup(sLights);
	lightPillarObject_.SetCamera(sMainCamera);

	lightPillarObject_.SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	XMVECTOR lightZ;

	void *constMap = nullptr;
	HRESULT result = lightConstBuff2->Map(0, nullptr, &constMap);
	std::memcpy(constMap, &tmpRate, sizeof(float));
	lightConstBuff2->Unmap(0, nullptr);

#pragma region lightAngleTest
	//float Zdot = upVec_.dot(sMainCamera->GetAngle());
	//float Xdot = upVec_.dot(sMainCamera->GetRight());
	//
	//if (abs(Xdot) <= abs(Zdot))
	//{
	//	GetMatRot(XMLoadFloat3(&sMainCamera->GetRight()), XMLoadFloat3(&upVec_), lightZ);
	//}
	//else
	//{
	//Vector3 tmpRight = upVec_.cross(sMainCamera->GetAngle());

	//	GetMatRot(XMLoadFloat3(&tmpRight), XMLoadFloat3(&upVec_), lightZ);

	//}
#pragma endregion

	Vector3 tmpRight = upVec_.cross(sMainCamera->GetAngle());

	GetMatRot(XMLoadFloat3(&tmpRight), XMLoadFloat3(&upVec_), lightZ);

	lightPillarObject_.SetRotationVector(lightZ, XMLoadFloat3(&upVec_));
}