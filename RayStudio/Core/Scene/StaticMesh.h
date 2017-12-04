#pragma once

#include "../../Core/Math/MathUtility.h"
#include "Material.h"
#include "../Vector.h"
#include "../Vector2D.h"

#include <vector>
using std::vector;

struct FStaticMeshVertex
{
	FVector Vertex;
	FVector Normal;
	FVector Tangent;
	FVector2D  Texcoord;
};

struct FStaticMeshSection
{
	uint16 material_index_;
	uint32 vertex_index_count_;
	uint32 polygen_index_count_;

	vector<uint16> v_polygen_index_list_;  //多边形索引表
	vector<uint16> v_vertex_index_list_;   //顶点索引表

	uint16 *ptr_polygen_index_;
	uint16 *ptr_vertex_index_;
};

class FStaticMesh
{
public:
	vector<FStaticMeshSection> v_mesh_section_;
	vector<FMaterial> v_mesh_material_;

	uint16 section_count_;
	uint32 vertex_count_;
	FStaticMeshVertex *ptr_vertex_;
public:
	FStaticMesh();
	~FStaticMesh();

};

