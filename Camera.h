#pragma once
#include <DirectXMath.h>
#include "Vector3.h"
namespace Projection
{
	struct ProjectionData
	{
		float angle;
		float width;
		float height;
		float screenNear;
		float screenFar;

		ProjectionData()
		{
			angle = 60.0f;
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
	Vector3 up;
	Vector3 shift;

	Camera();
	void Init(const DirectX::XMFLOAT3 &eye = { 0, 0, -100 }, const  DirectX::XMFLOAT3 &target = { 0, 0, 0 }, const  DirectX::XMFLOAT3 &up = { 0, 1, 0 }, const Projection::ProjectionData &projectionData = Projection::ProjectionData());
	void Update();

	//ÉJÉÅÉâÇàÍèuÇæÇØÇ∏ÇÁÇ∑
	void SetShift(DirectX::XMFLOAT3 shift);

	DirectX::XMFLOAT3 GetScreenPos(const DirectX::XMFLOAT3 &pos);
	DirectX::XMMATRIX GetMatBillboard() const;
	DirectX::XMMATRIX GetMatBillboardY() const;
	DirectX::XMMATRIX GetMatProjection() { return matProjection; }
	DirectX::XMMATRIX GetMatView() { return matView; }
	DirectX::XMMATRIX GetMatViewProj();
	DirectX::XMFLOAT3 GetEye(){return eye;}
	DirectX::XMFLOAT3 GetTarget(){return target;}
	DirectX::XMFLOAT3 GetAngle();
	void SetEye(const DirectX::XMFLOAT3 &eye){this->eye = eye;}
	void SetTarget(const DirectX::XMFLOAT3 &target){this->target = target;}
	void CameraRot(const DirectX::XMVECTOR &rotQ);
	void TargetRot(const DirectX::XMVECTOR &rotQ);
	void UpVecReset();
	//ïΩçsà⁄ìÆópä÷êî
	void MoveCamera(const DirectX::XMFLOAT3 &vec);
private:
	DirectX::XMMATRIX matProjection;

	DirectX::XMMATRIX matView;
	Vector3 eye;
	Vector3 target;
	DirectX::XMMATRIX matBillBoard;
	DirectX::XMMATRIX matBillBoardY;
	Projection::ProjectionData projectionData;

	void MakeMatCamera();

};