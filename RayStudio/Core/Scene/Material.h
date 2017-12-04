#pragma once
#include "../Vector4.h"

struct FMaterial
{
	FVector4    Ambient;
	FVector4    Diffuse;
	FVector4    Specular;   //��3��Ԫ��Ϊ���ʵľ��淴��ϵ������������ʱ���Ĺ⻬�̶�  
	FVector4    Emissive;   //�Է���


	void *diffuse_tex_;
	void *normal_tex_;
	void *specular_tex_;
	void *ao_tex_;
};
