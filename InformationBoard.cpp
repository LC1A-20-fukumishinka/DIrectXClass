#include "InformationBoard.h"
#include "TextureMgr.h"
#include "Collision/Collision.h"
#include "EaseClass.h"
#include <algorithm>
#include "ModelPhongPipeline.h"
#include "FukuMath.h"
using namespace std;
using namespace DirectX;
unique_ptr<Model> InformationBoard::sInformationModel;
unique_ptr<Model> InformationBoard::sBoardModel;
unique_ptr<PipeClass::PipelineSet> InformationBoard::sPipeline;
Camera *InformationBoard::sCamera = nullptr;
LightGroup *InformationBoard::sLightGroup = nullptr;
Vector3 InformationBoard::sBoardScale = Vector3(1.0f, 1.0f, 1.0f);

int InformationBoard::sBoardTextureHandle = -1;
InformationBoard::InformationBoard()
{
	if (!sInformationModel)
	{
		sInformationModel = std::make_unique<Model>();
		sInformationModel->CreateModel("InformationBoard");
	}
	if (!sBoardModel)
	{
		sBoardModel = std::make_unique<Model>();
		sBoardModel->CreateModel("QuestionMark");
		sBoardTextureHandle = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/white1x1.png");
	}
	if (!sPipeline)
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

		desc.VSname = L"Resources/shaders/InformationBoardVS.hlsl";
		desc.PSname = L"Resources/shaders/InformationBoardPS.hlsl";
		desc.inputLayout = inputLayout;
		desc.inputLayoutCount = _countof(inputLayout);
		desc.rootparams = rootparams;
		desc.rootparamsCount = _countof(rootparams);
		desc.blendName = GraphicsPipelineTypeName::BlendName::ALPHA;
		sPipeline = BaseGraphicsPipeline::CreatePipeLine(desc);
	}
}

InformationBoard::~InformationBoard()
{
}

void InformationBoard::Init(const wchar_t *filename, std::weak_ptr<Planet> basePlanet, const Vector3 &angle, const Vector3 &movePos, const Vector3 &scale)
{
	textureHandle_ = TextureMgr::Instance()->SpriteLoadTexture(filename);
	drawObject_.Init();
	drawObject_.SetCamera(sCamera);
	drawObject_.SetModel(sInformationModel.get());

	baseBoard_.Init();
	baseBoard_.SetCamera(sCamera);
	baseBoard_.SetLightGroup(sLightGroup);
	baseBoard_.SetModel(sBoardModel.get());
	baseScale_ = scale;
	drawObject_.SetScale(scale);
	baseBoard_.SetScale(sBoardScale);

	basePlanet_ = basePlanet;

	basePos_ = basePlanet.lock()->GetPos();

	Vector3 tmpAngle = angle;

	//元の惑星に位置を合わせる
	basePos_ += (tmpAngle.normalize() * basePlanet_.lock()->GetScale());
	movePos_ = movePos;

	baseBoard_.SetPosition(basePos_);


	Vector3 frontVec = Vector3(tmpAngle.y, -tmpAngle.x, 0.0f).cross(tmpAngle);

	XMVECTOR frontVecV = XMLoadFloat3(&frontVec);
	XMVECTOR upVecV = XMLoadFloat3(&tmpAngle.normalize());
	if (XMVector3Equal(frontVecV, upVecV))
	{
		frontVec = Vector3(0.0f, -1.0f, 0.0f);

		frontVecV = XMLoadFloat3(&frontVec);
	}
	baseBoard_.SetRotationVector(frontVecV, upVecV);
}


void InformationBoard::Update()
{
	float changeRate = 1.0f / 60.0f;
	if (isInDrawLength_)
	{
		objectScale_ += changeRate;
	}
	else
	{
		objectScale_ -= changeRate;
	}
	objectScale_ = std::clamp(objectScale_, 0.0f, 1.0f);

	float easeScale = Easing::easeOutExpo(objectScale_);
	easeScale = std::clamp(easeScale *= 1.1f, 0.0f, 1.0f);

	drawObject_.SetScale((baseScale_ * easeScale));
	baseBoard_.SetScale((sBoardScale * (1.0f - easeScale)));

	XMVECTOR bill = XMQuaternionRotationMatrix(sCamera->GetMatBillboard());
	Vector3 pos = XMVector3Rotate(XMLoadFloat3(&(movePos_ * easeScale)), bill);
	pos += basePos_;

	drawObject_.SetPosition(pos);
	drawObject_.SetRotation(bill);

	BoardUpdate();
}

void InformationBoard::Draw()
{
	if (!basePlanet_.lock()->GetIsSpawn()) return;
	baseBoard_.Update();
	baseBoard_.modelDraw(ModelPhongPipeline::Instance()->GetPipeLine(), true, sBoardTextureHandle);
	drawObject_.Update();
	drawObject_.modelDraw(sPipeline.get(), true, textureHandle_);
}

void InformationBoard::Finalize()
{
}

bool InformationBoard::CollisionPlayer(const Vector3 &pos)
{
	//     player,   informationBoard
	Sphere pSphere, iSphere;


	pSphere.center = XMLoadFloat3(&pos);
	pSphere.radius = 0.1f;

	iSphere.center = XMLoadFloat3(&basePos_);
	iSphere.radius = drawLength_;

	isInDrawLength_ = Collision::CheckSphere2Sphere(pSphere, iSphere);
	return isInDrawLength_;
}

void InformationBoard::SetCamera(Camera *camera)
{
	sCamera = camera;
}

void InformationBoard::BoardUpdate()
{
	Vector3 pos = basePos_;
	Vector3 boardUp = baseBoard_.GetUpVec();

	float move = 1.0f;

	idolAnimationRate_ += 0.01f;
	if (idolAnimationRate_ >= 1.0f)
	{
		idolAnimationRate_ = 0.0f;
		isIdolMotion = !isIdolMotion;
	}

	float rate = Easing::easeInOutQuad(idolAnimationRate_);

	if (isIdolMotion)
	{
		move *= rate;
	}
	else
	{
		move *= (1.0f - rate);
	}

	pos += (boardUp * move);
	baseBoard_.SetPosition(pos);

	XMVECTOR rot = baseBoard_.GetRotQuaternion();
	XMMATRIX addRotMat = XMMatrixRotationAxis(XMLoadFloat3(&boardUp), FukuMath::degree);
	rot =XMQuaternionMultiply(rot, XMQuaternionRotationMatrix(addRotMat));
	baseBoard_.SetRotation(rot);
}

void InformationBoard::SetLightGroup(LightGroup *lightGroup)
{
	sLightGroup = lightGroup;
}
