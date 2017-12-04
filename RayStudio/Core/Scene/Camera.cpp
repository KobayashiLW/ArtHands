#include "Camera.h"
#include <string>

Camera::Camera()
{
	//ptr_input_device_ = pDirectInputDevice;

	flag_com_ = COM_NONE;
	flag_cm_ = CM_CREATE_PERSON;

	cRight = FVector(0, 1, 0);
	
	memset(&mat_view_, 0, sizeof(FMatrix));
	memset(&mat_proj_, 0, sizeof(FMatrix));
	memset(&mat_inv_view_, 0, sizeof(FMatrix));
	memset(&mat_inv_proj_, 0, sizeof(FMatrix));
	memset(&mat_view_proj_, 0, sizeof(FMatrix));
	memset(&mat_inv_view_proj_, 0, sizeof(FMatrix));

	far_ = 0;
	near_ = 0;
	max_far_ = 0;
	min_near_ = 0;

	fov_ = 0;
	aspect_ratio_ = 0;

	yaw = 0;
	pitch = 0;
}

Camera::~Camera()
{
}

void Camera::SetLookAtPos(const FVector& Pos)
{
	if (!(flag_cm_ & CM_THIRD_PERSON)) return;

	static FVector dir;

	dir = cLook;

	dir *= r;

	cPos = cPos - dir;

}

void Camera::SetViewMatrix(const FVector& vPos,
	const FVector& vLookAt,
	const FVector& vUpDir)


{

	cPos = vPos;
	cLook = vLookAt;
	cUp = vUpDir;

	cLook.Normalize();
	cRight = FVector::Cross(cUp, cLook);
	cRight.Normalize();

	cUp = FVector::Cross(cLook, cRight);
	cUp.Normalize();

}

void Camera::SetProjMatrix(float fFovY,
	float fratio,
	float fzn,
	float fzf)
{

	xm_mat_proj_ = XMMatrixPerspectiveFovLH(
		fFovY,
		fratio,
		fzn,
		fzf);

	near_ = fzn;
	far_ = fzf;
	min_near_ = fzn;
	max_far_ = fzf;

	fov_ = fFovY;
	aspect_ratio_ = fratio;

	//计算透视投影逆矩阵

	float fny = 1 / tan(fFovY / 2);

	float fnx = fny / fratio;

	float a = fzf / (fzf - fzn);

	float b = -fzn * fzf / (fzf - fzn);

	FMatrix& V = mat_inv_proj_;

	memset(&V, 0, sizeof(FMatrix));

	V(0, 0) = 1 / fnx;

	V(1, 1) = 1 / fny;

	V(3, 2) = 1;

	V(2, 3) = 1 / b;

	V(3, 3) = -a / b;

}

void Camera::UpdateMouse(FVector pPos)
{
	float varYaw = 0.0f, varPitch = 0.0f;
	static FVector vPos = cPos;
	static FVector vLook = cLook;

	if (flag_cm_ == CM_THIRD_PERSON)
	{
		cLook = pPos;

		varYaw = (float)ptr_input_manager_->OffsetX * 0.005f;
		varPitch = (float)ptr_input_manager_->OffsetY * 0.005f;

		float x = vPos.X * cosf(varYaw) - vPos.Z * sinf(varYaw);
		float y1 = vPos.X * sinf(varYaw) + vPos.Z * cosf(varYaw);

		float y = vPos.Y * cosf(varPitch) - y1 * sinf(varPitch);
		float z = vPos.Y * sinf(varPitch) + y1  * cosf(varPitch);

		vPos = FVector(x, y, z);
		cPos = vPos + pPos;

		FMatrix roty = FMatrix::MatrixRotationY(varYaw);
		cRight.TransformCoord(roty);
		cRight.Normalize();
		FVector vLook = -vPos;
		vLook.Normalize();
		-cUp = FVector::Cross(vLook, cRight);
		cUp.Normalize();
	}
}

void Camera::Strafe(float units) // left/right
{
	FVector right = cRight;
	right *= units;
	cPos += right;
	cLook += right;
}

void Camera::Rise(float units)
{
	FVector var = cUp;
	var *= units;
	cPos += var;
	cLook += var;
}

void Camera::Walk(float units)   // forward/backward
{
	FVector dir;

}

void Camera::Pitch(float angle) // rotate on right vector
{

}
void Camera::Yaw(float angle)   // rotate on up vector
{

}


void Camera::Update(Frustum* frus)
{
	XMVECTOR Pos, Up, Look;
	Pos = XMLoadFloat3((XMFLOAT3*)&cPos);
	Up = XMLoadFloat3((XMFLOAT3*)&cUp);
	Look = XMLoadFloat3((XMFLOAT3*)&cLook);
	xm_mat_view_ = XMMatrixLookAtLH(Pos, Up, Look);
	xm_mat_view_proj_ = xm_mat_view_ * xm_mat_proj_;
}

Frustum Camera::CalculateFrustum(float fNear, float fFar)
{
	Frustum frustum;

	return frustum;
}