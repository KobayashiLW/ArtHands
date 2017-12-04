#pragma once
#include "../Core/Math/MathUtility.h"

#define MS_ALIGN(n) __declspec(align(n))
#define GCC_ALIGN(n)

struct FMatrix
{
public:
	union
	{
		MS_ALIGN(16) float M[4][4] GCC_ALIGN(16);
	};

	float& operator() (uint32 Row, uint32 Column) { return M[Row][Column]; }

	inline static FMatrix MatrixRotationY(float Radian);
};

inline FMatrix FMatrix::MatrixRotationY(float Radian)
{
	FMatrix M;

	float fSinAngle = sinf(Radian);
	float fCosAngle = cosf(Radian);

	M.M[0][0] = fCosAngle;
	M.M[0][1] = 0.0f;
	M.M[0][2] = -fSinAngle;
	M.M[0][3] = 0.0f;

	M.M[1][0] = 0.0f;
	M.M[1][1] = 1.0f;
	M.M[1][2] = 0.0f;
	M.M[1][3] = 0.0f;

	M.M[2][0] = fSinAngle;
	M.M[2][1] = 0.0f;
	M.M[2][2] = fCosAngle;
	M.M[2][3] = 0.0f;

	M.M[3][0] = 0.0f;
	M.M[3][1] = 0.0f;
	M.M[3][2] = 0.0f;
	M.M[3][3] = 1.0f;
	return M;
}