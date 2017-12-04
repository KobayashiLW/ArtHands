#pragma once
#include "ResourceManager.h"

class Actor
{
public:
	EntityMeshBuffer *ptr_entity_;

public:
	Actor();
	~Actor();

	void GetWorldTranslate();
};

