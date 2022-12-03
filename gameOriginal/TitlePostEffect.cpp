#include "TitlePostEffect.h"
#include <vector>
#include "MyDirectX.h"
#include "../imgui/ImguiManager.h"

TitlePostEffect::TitlePostEffect()
{
	obj.Init();

	PipelineDesc desc;

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = 
	{
		{
			"POSITION",									//�Z�}���e�B�b�N
			0,											//�����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X(0�ŗǂ�)
			DXGI_FORMAT_R32G32B32_FLOAT,				//�v�f���ƃr�b�g����\��(XYZ��3��float�^�Ȃ̂�"R32G32B32_FLOAT")
			0,											//���̓X���b�g�C���f�b�N�X(0�ł悢)
			D3D12_APPEND_ALIGNED_ELEMENT,				//�f�[�^�̃I�t�Z�b�g�l(D3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�)
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	//���̓f�[�^���(�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
			0											//��x�ɕ`�悷��C���X�^���X��(0�ł悢)
		},//(1�s�ŏ������������₷���炵��)
		//���W�ȊO�ɁA�F�A�e�N�X�`���Ȃǂ�n���ꍇ�͂���ɑ�����
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};


	//�f�X�N���v�^�e�[�u���̐ݒ�
	CD3DX12_DESCRIPTOR_RANGE  descRangeSRV;


	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 ���W�X�^
	//���[�g�p�����[�^�̐ݒ�
	CD3DX12_ROOT_PARAMETER rootparams[3] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);	//�萔�o�b�t�@�r���[�Ƃ��ď�����(b0)
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsConstantBufferView(2, 0,D3D12_SHADER_VISIBILITY_ALL);

	desc.VSname = L"Resources/shaders/TitleAnimationVS.hlsl";
	desc.PSname = L"Resources/shaders/TitleAnimationPS.hlsl";
	desc.inputLayout = inputLayout;
	desc.inputLayoutCount = _countof(inputLayout);
	desc.rootparams = rootparams;
	desc.rootparamsCount = _countof(rootparams);
	desc.blendName = GraphicsPipelineTypeName::BlendName::NONE;
	pipeline = BaseGraphicsPipeline::CreatePipeLine(desc);
}

TitlePostEffect::~TitlePostEffect()
{
}

void TitlePostEffect::Init()
{
HRESULT result;
MyDirectX *myD = MyDirectX::Instance();
	//�萔�o�b�t�@�̐���
	result = myD->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(TitleBuffer) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff2_)
	);
	TitleBuffer *constMap = nullptr;
	result = constBuff2_->Map(0, nullptr, (void **)&constMap);
	constMap->t = 0.0f;//�A�j���[�V�����̈ʒu
	constMap->type = Titletype;
	constBuff2_->Unmap(0, nullptr);
	animationTimer.Init(300);
	startTimer_ = 60;
}

void TitlePostEffect::Update()
{
	bool AnimationReset = false;
	ImGui::Begin("Title");
	ImGui::SetWindowSize(ImVec2(100, 100), ImGuiCond_::ImGuiCond_FirstUseEver);
	AnimationReset = ImGui::Button("AnimationStart");
	ImGui::RadioButton("Normal", &Titletype,0);
	ImGui::RadioButton("Up", &Titletype, 1);
	ImGui::RadioButton("Circle", &Titletype, 2);

	ImGui::End();

	HRESULT result;

	if (AnimationReset)
	{
		animationTimer.Reset();
	}
	if(startTimer_ <= 0)
	{
	animationTimer.Do(Easing::EaseMove::Out, Easing::Type::Expo);
	TitleBuffer *constMap = nullptr;
	result = constBuff2_->Map(0, nullptr, (void **)&constMap);
	constMap->t = animationTimer.Read();//�A�j���[�V�����̈ʒu
	constMap->type = Titletype;
	constBuff2_->Unmap(0, nullptr);
	}
	else
	{
		startTimer_--;
	}
}

void TitlePostEffect::Draw(int textureHandle)
{
	std::vector<int>textures;

	textures.emplace_back(textureHandle);

	obj.SetDrawBuffer(pipeline.get(), textures);
	MyDirectX::Instance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuff2_->GetGPUVirtualAddress());
	obj.DrawCommand();
}

void TitlePostEffect::Finalize()
{
}

void TitlePostEffect::Reset()
{
	animationTimer.Reset();
}
