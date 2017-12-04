#pragma once
#include "math.h"
#include "Math/MathUtility.h"

struct FMatrix;

class FVector
{
public:
	float X;
	float Y;
	float Z;

public:
	inline FVector() {}
	inline FVector(float InX, float InY, float InZ) : X(InX), Y(InY), Z(InZ){}

	inline bool operator == (const FVector &V) { return (X == V.X && Y == V.Y && Z == V.Z); }
	inline bool operator != (const FVector &V) { return (X != V.X || Y != V.Y || Z != V.Z); }
	inline FVector &operator /= (const float Scalar) { X /= Scalar; Y /= Scalar; Z /= Scalar; return (*this); }
	inline FVector &operator *= (const float Scalar) { X *= Scalar; Y *= Scalar; Z *= Scalar; return (*this); }
	inline FVector &operator += (const FVector &V) { X += V.X;  Y += V.Y; Z += V.Z; return (*this); }
	inline FVector &operator -= (const FVector &V) { X -= V.X;  Y -= V.Y; Z -= V.Z; return (*this); }

	inline FVector operator * (float Scalar) const { return FVector(X * Scalar, Y * Scalar, Z * Scalar); }
	inline FVector operator / (float Scalar) const { return FVector(X / Scalar, Y / Scalar, Z / Scalar); }
	inline FVector operator + (const FVector &V) const { return FVector(X + V.X, Y + V.Y, Z + V.Z); }
	inline FVector operator - (const FVector &V) const { return FVector(X - V.X, Y - V.Y, Z - V.Z); }
	inline FVector operator - (void) const { return FVector(-X, -Y, -Z); }

	inline float operator|(const FVector& V) const;
	inline FVector operator^(const FVector& V) const;

public:
	float Size() const;

	bool IsZero() const;

	bool Normalize(float Tolerance = SMALL_NUMBER);

	inline static float Dot(const FVector& A, const FVector& B);
	inline static FVector Cross(const FVector& A, const FVector& B);


	void TransformCoord(const FMatrix &M);
};

inline float FVector::Size(void) const
{ 
	return (float)FMath::Sqrt(X * X + Y * Y + Z * Z);
}

inline bool FVector::IsZero() const
{
	return X == 0.f && Y == 0.f && Z == 0.f;
}

inline bool FVector::Normalize(float Tolerance)
{
	const float SquareSum = X*X + Y*Y + Z*Z;
	if (SquareSum > Tolerance)
	{
		const float Scale = FMath::InvSqrt(SquareSum);
		X *= Scale; Y *= Scale; Z *= Scale;
		return true;
	}
	return false;
}

inline FVector FVector::Cross(const FVector& A, const FVector& B)
{
	return A ^ B;
}

inline float FVector::Dot(const FVector& A, const FVector& B)
{
	return A | B;
}


inline float FVector::operator|(const FVector& V) const
{
	return X*V.X + Y*V.Y + Z*V.Z;
}

inline FVector FVector::operator^(const FVector& V) const
{
	return FVector
		(
			Y * V.Z - Z * V.Y,
			Z * V.X - X * V.Z,
			X * V.Y - Y * V.X
			);
}
