#pragma once
#include "../Math/MathUtility.h"

struct CubeNode
{
public:

	float radius_;
	uint8 level;
	bool is_leaf_;
private:
	CubeNode  *ptr_child_[8];
public:
	void SetSize();
	void SetIsLeaf(bool isLeaf);
	void AddEntityIndex(int &_pEntityIndex);
};


class SceneOctree
{
public:
	SceneOctree();
	~SceneOctree();

	CubeNode  *ptr_root_;
	//Frustum	  *ptr_frustum_;

	uint8 max_level_;

public:
	void BuildOctree();
	void BuildChildNode();

	void ReleaseOctree();
	void ResetOctree();

	void Build();
	void TestNodeForOctree();
	void GenerousOctree();

	void InsertNode(CubeNode &Node);
	void InsertNodeFormActor();

	void Update();
};

