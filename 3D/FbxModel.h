#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

#include <fbxsdk.h>
struct Node
{
	//���O
	std::string name;
	//���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1, 1, 1, 0 };
	//���[�J����]�p
	DirectX::XMVECTOR rotation = { 0, 0, 0, 0 };
	//���[�J���ړ�
	DirectX::XMVECTOR translation = { 0, 0, 0, 1 };
	//���[�J���ό`�s��
	DirectX::XMMATRIX transform;
	//�O���[�o���ό`�s��
	DirectX::XMMATRIX globalTransform;
	//�e�m�[�h
	Node *parent = nullptr;
};
class FbxModel
{
public:
	//�t�����h�N���X
	friend class FbxLoader;
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	using string = std::string;
	template <class T> using vector = std::vector<T>;
public://�萔
	static const int MAX_BONE_INDICES = 4;
public:
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		UINT boneIndex[MAX_BONE_INDICES];	//�{�[�� �ԍ�
		float boneWeight[MAX_BONE_INDICES];	//�{�[�� �d��
	};

	struct Material
	{
		DirectX::XMFLOAT3 ambient = { 1, 1, 1 };
		DirectX::XMFLOAT3 diffuse = { 1, 1, 1 };
		//�e�N�X�`���[�p�̃n���h��
		int textureHandle;
	};

	struct Bone
	{
		//���O
		std::string name;
		//�����p���̋t�s��
		DirectX::XMMATRIX invInitialPose;
		//�N���X�^�[(FBX���̃{�[�����)
		FbxCluster *fbxCluster;
		//�R���X�g���N�^
		Bone(const std::string &name)
		{
			this->name = name;
		}
	};
public://�A�N�Z�b�T
	FbxScene *GetFbxScene(){return fbxScene;}

	std::vector<Bone> &GetBones() { return bones; }
public:
	~FbxModel();

	void CreateBuffers();
	void Draw();

	const XMMATRIX &GetModeTransform() { return meshNode->globalTransform; }
private:
	//���f����
	std::string name;
	//�m�[�h�z��
	std::vector<Node> nodes;
	//���b�V�������m�[�h
	Node *meshNode = nullptr;
	//���_�f�[�^�z��
	std::vector<VertexPosNormalUvSkin> vertices;
	//���_�C���f�b�N�X
	std::vector<unsigned short> indices;
	//�}�e���A��
	Material material;

	ComPtr<ID3D12Resource> vertBuff;
	ComPtr<ID3D12Resource> indexBuff;

	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	D3D12_INDEX_BUFFER_VIEW ibView = {};

	//�{�[���z��
	std::vector<Bone> bones;
	//FBX�V�[��
	FbxScene *fbxScene = nullptr;
};