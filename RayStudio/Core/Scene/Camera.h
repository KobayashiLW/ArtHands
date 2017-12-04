#pragma once
#include "../Math/MathUtility.h"
#include "../DXCommon.h"
#include "../Vector.h"
#include "../Matrix.h"
#include "../Vector2D.h"
#include "../Frustum.h"
#include "../../Editor/Common/InputManager.h"

//鼠标操作响应
enum CAMERA_ON_MOUSE
{

	COM_NONE = 0,			//不响应鼠标
	COM_LB = 1 << 0,		//响应左键
	COM_RB = 1 << 1,		//响应右键
	COM_AXIS_Z = 1 << 2		//响应鼠标滑轮

};
//摄像机模式
enum CAMERA_MODE
{
	CM_FIRST_PERSON = 0,		//第一人称
	CM_THIRD_PERSON = 1,		//第三人称
	CM_CLIP_AXIS_Y = 2,			//限定Y轴
	CM_CREATE_PERSON = 3

};

class Camera
{
public:
	uint32			flag_com_;
	uint32			flag_cm_;

	FVector		cPos; //摄像机位置
	FVector		cLook;
	FVector		cUp;
	FVector		cRight;

	FMatrix		mat_view_;
	FMatrix		mat_proj_;
	FMatrix		mat_view_proj_;
	FMatrix		mat_inv_view_;
	FMatrix		mat_inv_proj_;
	FMatrix		mat_inv_view_proj_;

	XMMATRIX	xm_mat_view_;
	XMMATRIX	xm_mat_proj_;
	XMMATRIX	xm_mat_view_proj_;
	XMMATRIX	xm_mat_inv_view_;
	XMMATRIX	xm_mat_inv_proj_;
	XMMATRIX	xm_mat_inv_view_proj_;

	bool			clicking;
	FVector2D		old_mouse_pt_;
	FVector2D		now_mouse_pt_;
	float			sensitivity;
	float			sensityvity_axisz_;

	float			r;
	long			axisz;
	long			dz;

	float			far_;
	float           near_;
	float           max_far_;
	float           min_near_;
	float			fov_;
	float           aspect_ratio_;
	double			yaw;
	double			pitch;
	
	InputManager *ptr_input_manager_;

public:
	Camera(InputManager *pInputManager) { ptr_input_manager_ = pInputManager; };
	Camera();
	~Camera();

	const FVector&	GetPos() { return cPos; }
	const FVector&	GetLookAt() { return cLook; }
	const FVector&	GetUpDir() { return cUp; }
	const FVector&	GetRightDir() { return cRight; }

	void SetPos(const FVector &var) { cPos = var; }
	void SetLookAt(const FVector &var) { cLook = var; }
	void SetUpDir(const FVector &var) { cUp = var; }
	void SetRightDir(const FVector &var) { cRight = var; }

	const FMatrix&	GetViewMatrix() { return mat_view_; }
	const FMatrix&	GetProjMatrix() { return mat_proj_; }
	const FMatrix&	GetInvProjMatrix() { return mat_inv_proj_; }
	const FMatrix&	GetViewProjMatrix() { return mat_view_proj_; }
	const FMatrix&	GetInvViewProjMatrix() { return mat_inv_view_proj_; }
	const FMatrix&	GetInvViewMatrix() { return mat_inv_view_; }

	float GetfarPlane() { return near_; }
	float GetFov() { return fov_; }


	void SetMode(uint32 CM) { flag_cm_ = CM; }

	void SetOnMouse(uint32 COM) { flag_com_ = COM; }

	void SetR(float r) { this->r = r; }

	void SetSensitivity(float s) { sensitivity = s; }

	void SetSensitivityAxisZ(float s) { sensityvity_axisz_ = s; }

	void SetLookAtPos(const FVector& Pos);

	void SetViewMatrix(const FVector& vPos, const FVector& vLookAt,
		const FVector& vUpDir);

	void SetProjMatrix(float fFovY, float fratio, float fnearPlane, float ffarPlane);

	void Update(Frustum* frus = NULL);

	void UpdateMouse(FVector pPos);

	//computes a frustum with given far and near planes
	Frustum CalculateFrustum(float fNear, float fFar);

	void Strafe(float units);
	void Walk(float units);
	void Rise(float units);
	void Pitch(float angle);
	void Yaw(float angle);
};

