#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>
#include <string>
#include <unordered_map>

struct VertexPosNormalUv
{
	DirectX::XMFLOAT3 pos; // xyz���W
	DirectX::XMFLOAT3 normal; // �@���x�N�g��
	DirectX::XMFLOAT2 uv;  // uv���W
};

struct ConstBufferDataB1
{
	DirectX::XMFLOAT3 ambient;	//�A���r�G���g�W��
	float pad1;			//�p�f�B���O
	DirectX::XMFLOAT3 diffuse;	//�f�B�t�[�Y�W��
	float pad2;			//�p�f�B���O
	DirectX::XMFLOAT3 specular;	//�X�y�L�����[�W��
	float alpha;		//�A���t�@
};

struct ModelObject
{
	//�}�e���A��
	struct Material
	{
		std::string name;
		DirectX::XMFLOAT3 ambient;
		DirectX::XMFLOAT3 diffuse;
		DirectX::XMFLOAT3 specular;
		float alpha;
		std::string textureFilename;

		//�R���X�g���N�^
		Material()
		{
			ambient = { 0.3f, 0.3f, 0.3f };
			diffuse = { 0.0f, 0.0f, 0.0f };
			specular = { 0.0f, 0.0f, 0.0f };
			alpha = 1.0f;
		}
	};

	std::vector<VertexPosNormalUv> vertices;
	std::vector<unsigned short> indices;

	// ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	//�}�e���A���p�̒萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;

	//�}�e���A��
	Material material;


	//�e�N�X�`���[�p�̃n���h��
	int textureHandle;

	//���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
};
class Model
{
public:
	Model();
	~Model();

	void CreateModel( const std::string &modelname, bool isSmoothing = false);
	void LoadModel(const std::string &directoryPath, const std::string &modelname, bool isSmoothing);
	void LoadMaterial(const std::string &directoryPath, const std::string &filename);
	void LoadTexture(const std::string &directoryPath, const std::string &filename);

	ModelObject *GetModel();
	void SetModel(const ModelObject &model);

	inline const std::vector<VertexPosNormalUv> & GetVertices(){return model.vertices;}
	inline const std::vector<unsigned short> &GetIndices(){return model.indices;}
private:
	/// <summary>
	/// �G�b�W�������f�[�^�̒ǉ�
	/// </summary>
	/// <param name="indexPosition">���W�C���f�b�N�X</param>
	/// <param name="indexVertex">���_�C���f�b�N�X</param>
	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);

	/// <summary>
	/// </summary>
	void CalculateSmoothVertexNormals();
private:
	ModelObject model;
};