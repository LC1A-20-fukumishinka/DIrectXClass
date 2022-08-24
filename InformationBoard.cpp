#include "InformationBoard.h"
#include "TextureMgr.h"

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
		// ���_���C�A�E�g
		D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ // xy���W(1�s�ŏ������ق������₷��)
				"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
				"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // uv���W(1�s�ŏ������ق������₷��)
				"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
		};

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
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
	desc.blendName = GraphicsPipelineTypeName::BlendName::ADD;
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
	drawObject_.SetScale(scale);
	basePos_ = basePos;
	movePos_ = movePos;
}

void InformationBoard::Update()
{
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

void InformationBoard::SetCamera(Camera *camera)
{
	sCamera = camera;
}

void InformationBoard::SetLightGroup(LightGroup *lightGroup)
{
sLightGroup = lightGroup;
}
