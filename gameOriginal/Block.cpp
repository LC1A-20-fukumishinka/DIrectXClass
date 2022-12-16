#include "Block.h"
#include "Model.h"
#include <array>
#include "FukuMath.h"
std::unique_ptr<Model> Block::sBlockModel;
using namespace DirectX;
using namespace FukuMath;
Block::Block()
{
	if (!sBlockModel)
	{
		sBlockModel = std::make_unique<Model>();
		sBlockModel->CreateModel("chr_sword");
	}
}

Block::~Block()
{
}

void Block::Init(std::weak_ptr<Planet> basePlanet,const Vector3 &angle, float scale)
{
	ObjectOnSphere::Init(basePlanet, angle, scale);
	SetModel(sBlockModel.get());
}

std::vector<Triangle> Block::GetCollisionPlanes()
{
	std::vector<Triangle> BlockPlanes;

	float radius = GetWorldScale() / 2;

	XMVECTOR rotQ = GetRotationQ();
	XMVECTOR upV = XMVector3Rotate(YVec, rotQ);
	XMVECTOR rightV = XMVector3Rotate(XVec, rotQ);
	XMVECTOR frontV = XMVector3Rotate(ZVec, rotQ);
	XMVECTOR basePos = F3toV(GetWorldPos());
	upV *= radius;
	rightV *= radius * 5.0f;
	frontV *= radius * 5.0f;
	std::array<Triangle, 2> upTriangle;//上
	upTriangle[0].p0 = { upV - rightV + frontV  + basePos};//{-x, y, z}
	upTriangle[0].p1 = { upV + rightV + frontV  + basePos};//{ x, y, z}
	upTriangle[0].p2 = { upV + rightV - frontV  + basePos};//{ x, y,-z}
	upTriangle[0].ComputeNormal();
	upTriangle[1].p0 = { upV - rightV + frontV  + basePos};//{-x, y, z}
	upTriangle[1].p2 = { upV + rightV - frontV  + basePos};//{ x, y,-z}
	upTriangle[1].p1 = { upV - rightV - frontV  + basePos};//{-x, y,-z}
	upTriangle[1].ComputeNormal();
	for (auto &e : upTriangle)
	{
		BlockPlanes.push_back(e);
	}
	std::array<Triangle, 2> downTriangle;//下
	downTriangle[0].p0 = { -upV - rightV - frontV  + basePos};//{-x,-y,-z}
	downTriangle[0].p1 = { -upV - rightV + frontV  + basePos};//{-x,-y, z}
	downTriangle[0].p2 = { -upV + rightV - frontV  + basePos};//{ x,-y,-z}
	downTriangle[0].ComputeNormal();
	downTriangle[1].p0 = { -upV - rightV - frontV  + basePos};//{-x,-y,-z}
	downTriangle[1].p1 = { -upV + rightV - frontV  + basePos};//{ x,-y,-z}
	downTriangle[1].p2 = { -upV + rightV - frontV  + basePos};//{ x,-y, z}
	downTriangle[1].ComputeNormal();
	for (auto &e : downTriangle)
	{
		BlockPlanes.push_back(e);
	}
	std::array<Triangle, 2> rightTriangle;//右
	rightTriangle[0].p0 = { upV + rightV + frontV  + basePos};//{ x, y, z}
	rightTriangle[0].p1 = {-upV + rightV + frontV  + basePos};//{ x,-y, z}
	rightTriangle[0].p2 = {-upV + rightV - frontV  + basePos};//{ x,-y,-z}
	rightTriangle[0].ComputeNormal();
	rightTriangle[1].p0 = { upV + rightV + frontV  + basePos};//{ x, y, z}
	rightTriangle[1].p1 = {-upV + rightV - frontV  + basePos};//{ x,-y,-z}
	rightTriangle[1].p2 = { upV + rightV - frontV  + basePos};//{ x, y,-z}
	rightTriangle[1].ComputeNormal();
	for (auto &e : rightTriangle)
	{
		BlockPlanes.push_back(e);
	}
	std::array<Triangle, 2> leftTriangle;//左
	leftTriangle[0].p0 = {-upV - rightV + frontV  + basePos};//{-x,-y, z}
	leftTriangle[0].p1 = { upV - rightV + frontV  + basePos};//{-x, y, z}
	leftTriangle[0].p2 = { upV - rightV - frontV  + basePos};//{-x, y,-z}
	leftTriangle[0].ComputeNormal();
	leftTriangle[1].p0 = {-upV - rightV + frontV  + basePos};//{-x,-y, z}
	leftTriangle[1].p1 = { upV - rightV - frontV  + basePos};//{-x, y,-z}
	leftTriangle[1].p2 = {-upV - rightV - frontV  + basePos};//{-x,-y,-z}
	leftTriangle[1].ComputeNormal();
	for (auto &e : leftTriangle)
	{
		BlockPlanes.push_back(e);
	}
	std::array<Triangle, 2> frontTriangle;//奥
	frontTriangle[0].p0 = {-upV - rightV + frontV  + basePos};//{-x,-y, z}
	frontTriangle[0].p1 = {-upV + rightV + frontV  + basePos};//{ x,-y, z}
	frontTriangle[0].p2 = { upV + rightV + frontV  + basePos};//{ x, y, z}
	frontTriangle[0].ComputeNormal();
	frontTriangle[1].p0 = {-upV - rightV + frontV  + basePos};//{-x,-y, z}
	frontTriangle[1].p1 = { upV + rightV + frontV  + basePos};//{ x, y, z}
	frontTriangle[1].p2 = { upV - rightV + frontV  + basePos};//{-x, y, z}
	frontTriangle[1].ComputeNormal();
	for (auto &e : frontTriangle)
	{
		BlockPlanes.push_back(e);
	}
	std::array<Triangle, 2> backTriangle;//手前
	backTriangle[0].p0 = { upV - rightV - frontV  + basePos};//{-x, y,-z}
	backTriangle[0].p1 = { upV + rightV - frontV  + basePos};//{ x, y,-z}
	backTriangle[0].p2 = {-upV + rightV - frontV  + basePos};//{ x,-y,-z}
	backTriangle[0].ComputeNormal();
	backTriangle[1].p0 = { upV - rightV - frontV  + basePos};//{-x, y,-z}
	backTriangle[1].p1 = {-upV + rightV - frontV  + basePos};//{ x,-y,-z}
	backTriangle[1].p2 = {-upV - rightV - frontV  + basePos};//{-x, y,-z}
	backTriangle[1].ComputeNormal();
	for (auto &e : backTriangle)
	{
		BlockPlanes.push_back(e);
	}
	return BlockPlanes;
}

//void Block::CollisionPlayer(float radius, const Vector3 pos, const Vector3 up)
//{
//	//矩形のせいせい
//
//	//乗るための矩形の判定
//
//	//横に当たる判定(押せる？)
//}
