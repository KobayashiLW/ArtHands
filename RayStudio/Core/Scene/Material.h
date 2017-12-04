#pragma once
#include "../Vector4.h"

struct FMaterial
{
	FVector4    Ambient;
	FVector4    Diffuse;
	FVector4    Specular;   //第3个元素为材质的镜面反射系数，即代表材质表面的光滑程度  
	FVector4    Emissive;   //自发光


	void *diffuse_tex_;
	void *normal_tex_;
	void *specular_tex_;
	void *ao_tex_;
};
