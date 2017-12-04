#include "Vector.h"
#include "Matrix.h"

void FVector::TransformCoord(const FMatrix &M)
{
	float x =  X * M.M[0][0] + Y * M.M[1][0] + Z * M.M[2][0] + M.M[3][0];
	float y =  X * M.M[0][1] + Y * M.M[1][1] + Z * M.M[2][1] + M.M[3][1];
	float z =  X * M.M[0][2] + Y * M.M[1][2] + Z * M.M[2][2] + M.M[3][2];
	X = x;
	Y = y;
	Z = z;
	/*return FVector
		(
			A.X * M.M[0][0] + A.Y * M.M[1][0] + A.Z * M.M[2][0] + M.M[3][0],
			A.X * M.M[0][1] + A.Y * M.M[1][1] + A.Z * M.M[2][1] + M.M[3][1],
			A.X * M.M[0][2] + A.Y * M.M[1][2] + A.Z * M.M[2][2] + M.M[3][2]
			);*/
}