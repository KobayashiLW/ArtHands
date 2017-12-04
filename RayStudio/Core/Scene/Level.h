#pragma once
#include "SceneOctree.h"

#include <vector>

using std::vector;

class Actor;

class Level
{
public:
	vector<Actor *> Actors;

	SceneOctree Octree;
public:
	Level();
	~Level();

	void LoadLevel();
};

