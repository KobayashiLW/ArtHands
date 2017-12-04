#include "SceneOctree.h"

#define OCTREE_MAX_LEVEL 8

void CubeNode::SetSize()
{
	//aabb_ = aabb;
	for (uint8 i = 0; i < 8; i++)
		ptr_child_[i] = NULL;
	is_leaf_ = false;
}

void CubeNode::SetIsLeaf(bool isLeaf)
{
	is_leaf_ = isLeaf;
}



//////////////////////////////////////////////////////////////////////////////////////////
SceneOctree::SceneOctree()
{
}


SceneOctree::~SceneOctree()
{
}


void SceneOctree::BuildOctree()
{
	//aabb_.Set(Vector3(inf, inf, inf), Vector3(-inf, -inf, -inf));

}

void SceneOctree::BuildChildNode()
{

}

void SceneOctree::InsertNode(CubeNode &node)
{

}

void SceneOctree::InsertNodeFormActor()
{

}

void SceneOctree::Update()
{

}