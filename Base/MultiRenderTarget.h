#pragma once
#include <DirectXMath.h>
#include <IGraphicsPipeline.h>
#include <vector>

class MultiRenderTarget
{
public:
	/// <summary>
	///�R���X�g���N�^
	/// </summary>
	MultiRenderTarget();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="targetCount">��������^�[�Q�b�g�̌�</param>
	void Init(int targetCount);

	void PreDrawScene();

	void PostDrawScene();

	//�e�N�X�`���o�b�t�@�̔z���n��(�g��Ȃ�����)
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> GetTexBuff();

	//�e�N�X�`���}�l�[�W�����̃e�N�X�`���ԍ���Ԃ�
	int GetTextureNum(int renderTargetNum);

	//�[�x�n���Z�b�g
	void DepthReset();
private:

	void MakeRTV();

	void MakeDepthBuffer();

	void MakeDSV();
private:
//�����_�[�^�[�Q�b�g�̐�
	enum class RenderTargetCount
	{
		count = 1,
	};
private:
	int targetCount_;

	ID3D12GraphicsCommandList *cmdList = nullptr;

	//�e�N�X�`���o�b�t�@
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> texBuff;
	//�e�N�X�`���ԍ�
	std::vector<int> textureNums;

	//�[�x�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;

	//RTV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;

	//DSV�p�̃f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	//��ʃN���A�J���[
	static const float clearColor[4];

};
