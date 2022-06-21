#include "MeshCollider.h"
#include "Model.h"
#include "../Collision/Collision.h"

using namespace DirectX;
void MeshCollider::ConstructTraiangles(Model *model)
{
	//�O�p�`���X�g���N���A
	triangles.clear();

	const std::vector<unsigned short> indices = model->GetIndices();
	const std::vector<VertexPosNormalUv> vertices = model->GetVertices();
	size_t triangleNum = (indices.size() / 3);
	//���݂̃��b�V���̎O�p�`�̐������A�O�p�`���X�g�ɃX�y�[�X��ǉ�����
	triangles.resize(triangles.size() + triangleNum);
	for (int i = 0; i < triangleNum; i++)
	{
		Triangle &tri = triangles[i];
		int idx0 = indices[i * 3 + 0];
		int idx1 = indices[i * 3 + 1];
		int idx2 = indices[i * 3 + 2];

		//�O�p�`�̂R���_�̍��W����
		tri.p0 = { vertices[idx0].pos.x, vertices[idx0].pos.y, vertices[idx0].pos.z, 1 };
		tri.p1 = { vertices[idx1].pos.x, vertices[idx1].pos.y, vertices[idx1].pos.z, 1 };
		tri.p2 = { vertices[idx2].pos.x, vertices[idx2].pos.y, vertices[idx2].pos.z, 1 };
		//�R���_����@�����v�Z
		tri.ComputeNormal();
	}
}

void MeshCollider::Update()
{
invMatWorld = XMMatrixInverse(nullptr, GetObject3D()->GetMatWorld());
}

bool MeshCollider::CheckCollisionSphere(const Sphere &sphere, DirectX::XMVECTOR *inter)
{
Sphere localSphere;
localSphere.center = XMVector3Transform(sphere.center, invMatWorld);
localSphere.radius *= XMVector3Length(invMatWorld.r[0]).m128_f32[0];

//���[�J�����W�n�ŃN�I���`�F�b�N
	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for (; it != triangles.cend(); ++it)
	{
		const Triangle &triangle = *it;

		//���ƎO�p�`�̓����蔻��
		if (Collision::CheckSphere2Triangle(localSphere, triangle, inter))
		{
			if (inter)
			{
				const XMMATRIX &matWorld = GetObject3D()->GetMatWorld();
				//���[���h���W�n�ł̌�_�𓾂�
				*inter = XMVector3Transform(*inter, matWorld);
			}
			return true;
		}
	}
	return false;
}

bool MeshCollider::CheclCollisionRay(const Ray &ray, float *distance, DirectX::XMVECTOR *inter)
{
	Ray localRay;
	localRay.start = XMVector3Transform(ray.start, invMatWorld);
	localRay.dir = XMVector3TransformNormal(ray.dir, invMatWorld);

	//���[�J�����W�n�ōl�����`�F�b�N
	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for (; it != triangles.cend(); ++it)
	{
		const Triangle &triangle = *it;
		XMVECTOR tempInter;
		//���C�ƎO�p�`�̓����蔻��
		if (Collision::CheckRay2Triangle(localRay, triangle, nullptr, &tempInter))
		{
			const XMMATRIX &matWorld = GetObject3D()->GetMatWorld();
			//���[���h���W�n�Ō�_�𓾂�
			tempInter = XMVector3Transform(tempInter, matWorld);

			if (distance)
			{//��_�g���C�x�X�̋������v�Z
				XMVECTOR sub = tempInter - ray.start;
				*distance = XMVector3Dot(sub, ray.dir).m128_f32[0];
			}
			if (inter)
			{
				*inter = tempInter;
			}
			return true;
		}
	}
	return false;
}
