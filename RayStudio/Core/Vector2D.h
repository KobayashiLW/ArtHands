#pragma once
#include "math.h"
#include "Math/MathUtility.h"

struct FVector2D
{
	float X;
	float Y;

	inline FVector2D() {}
	inline FVector2D(float InX, float InY) : X(InX), Y(InY){}

	inline bool operator == (const FVector2D &V) { return (X == V.X && Y == V.Y); }
	inline bool operator != (const FVector2D &V) { return (X != V.X || Y != V.Y); }
	inline FVector2D &operator /= (const float Scalar) { X /= Scalar; Y /= Scalar; return (*this); }
	inline FVector2D &operator *= (const float Scalar) { X *= Scalar; Y *= Scalar; return (*this); }
	inline FVector2D &operator += (const FVector2D &V) { X += V.X;  Y += V.Y; return (*this); }
	inline FVector2D &operator -= (const FVector2D &V) { X -= V.X;  Y -= V.Y; return (*this); }

	inline FVector2D operator * (float Scalar) const { return FVector2D(X * Scalar, Y * Scalar); }
	inline FVector2D operator / (float Scalar) const { return FVector2D(X / Scalar, Y / Scalar); }
	inline FVector2D operator + (const FVector2D &V) const { return FVector2D(X + V.X, Y + V.Y); }
	inline FVector2D operator - (const FVector2D &V) const { return FVector2D(X - V.X, Y - V.Y); }
	inline FVector2D operator - (void) const { return FVector2D(-X, -Y); }

	inline float operator|(const FVector2D& V) const;
	inline float operator^(const FVector2D& V) const;

public:
	float Size() const;

	bool IsZero() const;

	void Normalize(float Tolerance = SMALL_NUMBER);

	inline static float Dot(const FVector2D& A, const FVector2D& B);
	inline static float Cross(const FVector2D& A, const FVector2D& B);
};

inline float FVector2D::Size(void) const
{
	return (float)FMath::Sqrt(X * X + Y * Y);
}

inline bool FVector2D::IsZero() const
{
	return X == 0.f && Y == 0.f;
}

inline void FVector2D::Normalize(float Tolerance)
{
	const float SquareSum = X*X + Y*Y;
	if (SquareSum > Tolerance)
	{
		const float Scale = FMath::InvSqrt(SquareSum);
		X *= Scale;
		Y *= Scale;
		return;
	}
	X = 0.0f;
	Y = 0.0f;
}

inline float FVector2D::Cross(const FVector2D& A, const FVector2D& B)
{
	return A ^ B;
}

inline float FVector2D::Dot(const FVector2D& A, const FVector2D& B)
{
	return A | B;
}

inline float FVector2D::operator|(const FVector2D& V) const
{
	return X*V.X + Y*V.Y;
}


inline float FVector2D::operator^(const FVector2D& V) const
{
	return X*V.Y - Y*V.X;
}