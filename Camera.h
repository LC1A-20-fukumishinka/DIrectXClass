#pragma once
#include <DirectXMath.h>
#include "Vector3.h"
namespace Projection
{
	struct ProjectionData
	{
		float startAngle;
		float width;
		float height;
		float screenNear;
		float screenFar;

		ProjectionData()
		{
			startAngle = 60.0f;
			width = 0.0f;
			height = 0.0f;
			screenNear = 0.1f;
			screenFar = 1000.0f;
		}
	};

}
class Camera
{
public:
	Vector3 target;
	Vector3 up;
	Vector3 position;
	Vector3 shift;

	Camera();
	void Init(const DirectX::XMFLOAT3 &eye = { 0, 0, -100 }, const  DirectX::XMFLOAT3 &target = { 0, 0, 0 }, const DirectX::XMFLOAT3 &position = { 0, 0, 0 }, const  DirectX::XMFLOAT3 &up = { 0, 1, 0 }, Projection::ProjectionData &projectionData = Projection::ProjectionData());
	void Update();

	//ƒJƒƒ‰‚ðˆêu‚¾‚¯‚¸‚ç‚·
	void SetShift(DirectX::XMFLOAT3 shift);

	DirectX::XMFLOAT3 GetScreenPos(const DirectX::XMFLOAT3 &pos);
	DirectX::XMMATRIX GetMatBillboard() const;
	DirectX::XMMATRIX GetMatBillboardY() const;
	DirectX::XMMATRIX GetMatProjection() { return matProjection; }
	DirectX::XMMATRIX GetMatView() { return matView; }
	DirectX::XMMATRIX GetMatViewProj();
	DirectX::XMFLOAT3 GetEye(){return eye;}
	void SetEye(const DirectX::XMFLOAT3 &eye){this->eye = eye;}
private:
	DirectX::XMMATRIX matProjection;

	DirectX::XMMATRIX matView;
	Vector3 eye;
	DirectX::XMMATRIX matBillBoard;
	DirectX::XMMATRIX matBillBoardY;
	Projection::ProjectionData projectionData;

	void MakeMatCamera();

};