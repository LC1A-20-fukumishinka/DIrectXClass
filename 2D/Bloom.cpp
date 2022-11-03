#include "Bloom.h"
#include <vector>
#include "MyDirectX.h"
//#include "../imgui/ImguiManager.h"
Bloom::Bloom()
{
	float tmp = 10.0f;

	bloomTargets.resize(5);
	for (auto &e : bloomTargets)
	{
		float count = static_cast<float>(&e - &bloomTargets[0]);
		count += 1.0f;
		e.Init(1, Vector3((1280.0f / powf(2.0f, count)), (720.0f / powf(2.0f, count)), 0.0f));
	}
	//�`��p�I�u�W�F�N�g��������
	brightnessExtractionObj.Init();

	bloomObj.Init();
	//�p�C�v���C���������\���̂�錾
	PipelineDesc desc;

	//input �\���̂����
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

	//�f�X�N���v�^�e�[�u���̐ݒ胁�C���ƃu���[��
	CD3DX12_DESCRIPTOR_RANGE  mainTextureDescRangeSRV;
	CD3DX12_DESCRIPTOR_RANGE  brightDescRangeSRV;

	mainTextureDescRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 ���W�X�^
	brightDescRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);	//t1 ���W�X�^

	//���[�g�p�����[�^�̐ݒ�
	CD3DX12_ROOT_PARAMETER rootparams[3] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &mainTextureDescRangeSRV, D3D12_SHADER_VISIBILITY_ALL); //���C���̕`��e�N�X�`��
	rootparams[2].InitAsDescriptorTable(1, &brightDescRangeSRV, D3D12_SHADER_VISIBILITY_ALL); //�u���[���̕`��e�N�X�`��


	desc.VSname = L"Resources/shaders/AddColorVS.hlsl";
	desc.PSname = L"Resources/shaders/AddColorPS.hlsl";
	desc.inputLayout = inputLayout;
	desc.inputLayoutCount = _countof(inputLayout);
	desc.rootparams = rootparams;
	desc.rootparamsCount = _countof(rootparams);
	desc.blendName = GraphicsPipelineTypeName::BlendName::NONE;
	bloomPipeline = BaseGraphicsPipeline::CreatePipeLine(desc);

	PipelineDesc GaussianBlurDesc;


	//�f�X�N���v�^�e�[�u���̐ݒ�
	CD3DX12_DESCRIPTOR_RANGE  bloomBaseDescRangeSRV;
	bloomBaseDescRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 ���W�X�^
	CD3DX12_DESCRIPTOR_RANGE  bloomBrightDescRangeSRV;
	bloomBrightDescRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);	//t1 ���W�X�^

	//���[�g�p�����[�^�̐ݒ�
	CD3DX12_ROOT_PARAMETER bloomRootparams[2] = {};
	bloomRootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	bloomRootparams[1].InitAsDescriptorTable(1, &bloomBaseDescRangeSRV, D3D12_SHADER_VISIBILITY_ALL); //�ʏ�̕`��


	GaussianBlurDesc.VSname = L"Resources/shaders/GaussianblurVS.hlsl";
	GaussianBlurDesc.PSname = L"Resources/shaders/GaussianblurPS.hlsl";
	GaussianBlurDesc.inputLayout = inputLayout;
	GaussianBlurDesc.inputLayoutCount = _countof(inputLayout);
	GaussianBlurDesc.rootparams = bloomRootparams;
	GaussianBlurDesc.rootparamsCount = _countof(bloomRootparams);
	GaussianBlurDesc.blendName = GraphicsPipelineTypeName::BlendName::NONE;
	GaussianBlurDesc.loopMode = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	GaussianBlurPipeline = BaseGraphicsPipeline::CreatePipeLine(GaussianBlurDesc);
}

Bloom::~Bloom()
{
}

void Bloom::Init()
{
}

void Bloom::Update()
{
}

void Bloom::BrightUpdate(int brightTextureHandle)
{
	int drawTexture = brightTextureHandle;

	for (auto &e : bloomTargets)
	{
		std::vector<int>textures;

		textures.emplace_back(drawTexture);

		e.PreDrawScene();
		brightnessExtractionObj.SetDrawBuffer(GaussianBlurPipeline.get(), textures);
		brightnessExtractionObj.DrawCommand();
		e.PostDrawScene();
		drawTexture = e.GetTextureNum(0);
	}
}

void Bloom::Draw(int mainTextureHandle)
{
	{
		std::vector<int>textures;
		textures.emplace_back(mainTextureHandle);
		textures.emplace_back(bloomTargets[4].GetTextureNum(0));
		bloomObj.Draw(bloomPipeline.get(), textures);
	}
}

void Bloom::Finalize()
{
}
