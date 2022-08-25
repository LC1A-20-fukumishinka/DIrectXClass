#include "InformationBoard.h"
#include "TextureMgr.h"
#include "Collision/Collision.h"
#include "EaseClass.h"
#include <algorithm>
using namespace std;
using namespace DirectX;
unique_ptr<Model> InformationBoard::sModel;
unique_ptr<PipeClass::PipelineSet> InformationBoard::sPipeline;
Camera *InformationBoard::sCamera = nullptr;
LightGroup *InformationBoard::sLightGroup = nullptr;
InformationBoard::InformationBoard()
{
	if (!sModel)
	{
		sModel = std::make_unique<Model>();
		sModel->CreateModel("InformationBoard");
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

void InformationBoard::Init(const wchar_t *filename, const Vector3 &basePos, const Vector3 &movePos, const Vector3 &scale)
{
	drawObject_.Init();
	textureHandle_ = TextureMgr::Instance()->SpriteLoadTexture(filename);
	drawObject_.SetCamera(sCamera);
	drawObject_.SetLightGroup(sLightGroup);
	drawObject_.SetModel(sModel.get());
	baseScale_ = scale;
	drawObject_.SetScale(scale);
	basePos_ = basePos;
	movePos_ = movePos;
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
	objectScale_ = std::clamp(objectScale_,0.0f, 1.0f);

	float easeScale = Easing::easeOutExpo(objectScale_);
	easeScale = std::clamp(easeScale *= 1.1f, 0.0f, 1.0f);

	drawObject_.SetScale((baseScale_ * easeScale));

	XMVECTOR bill = XMQuaternionRotationMatrix(sCamera->GetMatBillboard());
	Vector3 pos = XMVector3Rotate(XMLoadFloat3(&movePos_), bill);
	pos += basePos_;

	drawObject_.SetPosition(pos);
	drawObject_.SetRotation(bill);
}

void InformationBoard::Draw()
{
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

void InformationBoard::SetLightGroup(LightGroup *lightGroup)
{
sLightGroup = lightGroup;
}
