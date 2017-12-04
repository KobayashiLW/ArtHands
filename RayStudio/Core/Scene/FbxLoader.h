#pragma once
#include <vector>
#include <string>
#include <fbxsdk.h>

#include "Material.h"
#include "../Vector.h"
#include "../Vector2D.h"
#include "../Math/MathUtility.h"
#include "StaticMesh.h"

using std::vector;
using std::wstring;
using std::string;

struct FbxSdkVersion
{
	int mMajor, mMinor, mRevision;
};

enum MeshType
{
	MT_StaticMesh,
	MT_SkeletalMesh
};

struct FbxSubMeshDate
{
	//Material *ptr_material_;

	vector<int> v_polygen_index_list_;  //多边形索引表
	vector<int> v_vertex_index_list_;   //顶点索引表


};

struct FbxRawMeshVertexFormat
{
	FVector *ptr_vertex_;
	FVector *ptr_normal_;
	FVector *ptr_tangent_;
	FVector *ptr_tex_;

};

struct FbxRawVertexDate
{
	FVector vertex;
	FVector normal;
	FVector tangent;
	FVector tex;

	int vertex_index_;

	//the mask if is true mean repeat that will delete
	bool mask;
	int point_to_vertex_index_;

	bool tex_maping_mode_;  //true is eByControlPoint  false is eByPolygonVertex
	bool tex_reference_mode_; //true is eDirect  false is eIndexToDirect
	int	tex_index_;

	bool normal_maping_mode_;  //true is eByControlPoint  false is eByPolygonVertex
	bool normal_reference_mode_; //true is eDirect  false is eIndexToDirect
	int normal_index_;

	bool tangent_maping_mode_;  //true is eByControlPoint  false is eByPolygonVertex
	bool tangentx_reference_mode_; //true is eDirect  false is eIndexToDirect
	int tangent_index_;

};

struct FbxRawVertex
{
	FVector Vertex;
	FVector Normal;
	FVector Tangent;
	FVector2D  Texcoord;

	bool Mask;
	int PointToVertexIndex;

	int VertexIndex;
	int NoramlIndex;
	int TangentIndex;
	int TexcoordIndex;
};

struct FbxStaticMesh
{
	FStaticMesh *pStaticMesh;
	FStaticMeshVertex *pVertex;
};

struct FbxMeshDate
{
public:
	//fbx file path and file name
	string path_;
	string name_;

	//global vertex and triangle count
	uint32_t vertex_count_;
	uint32_t triangle_count_;

	//triangle index buffer
	uint32_t *ptr_index_;
	//Vector3  *ptr_control_point_;
	//material
	uint8_t material_count_;

	//sub mesh 当材质数为0时设置子网格为
	vector<FbxSubMeshDate> v_sub_mesh_;

	//raw mesh date
	FbxRawVertexDate *ptr_raw_mesh_;
	FbxRawVertexDate *ptr_vertex_buffer_;

	//Box
	//BoundingBox bound_box_;

	//
	MeshType mt_;
};

class TextureManager;
class FbxLoader
{
public:
	FbxLoader();
	FbxLoader(TextureManager *pTextureManager);
	~FbxLoader();

	bool Initialize();
	bool LoadScene(const string *filePath);

	vector<FStaticMesh *> ReadMeshFormFbx(const string *filePath);

	bool ReadMaterial(FbxMesh* pMesh);
	bool ReadMaterialAttribute(FbxSurfaceMaterial* pSurfaceMaterial, FMaterial &Material);
	//ID3D11ShaderResourceView *LoadMaterialTexture(const char* tfilename);

	void ProcessNode(FbxNode* pNode);
	void ProcessMesh(FbxNode* pNode);

	inline FbxVector4 FbxVector4Transform(FbxVector4 &V, FbxMatrix &M);
	inline FbxVector4 FbxVector4TransformCoord(FbxVector4 &V, FbxMatrix &M);

	//BoundingBox FbxComputeBoundingBox(Vector3 *pV, int vCount);

	inline void ReadUV(FbxLayerElementUV* pTexCoords, int CtrlPointIndex, int TexUVIndex, FbxRawVertex &RawVertex);
	inline void ReadNormal(FbxLayerElementNormal* pNormal, int CtrlPointIndex, int VertexCounter, FbxRawVertex &RawVertex);
	inline void ReadTangent(FbxLayerElementTangent* pTangent, int CtrlPointIndex, int VertexCounter, FbxRawVertex &RawVertex);

	void ReadSkinAnim(FbxMesh* pMesh);
	void Release();
public:

	FbxManager		 *ptr_fbx_manager_;
	FbxScene		 *ptr_fbx_scene_;
	TextureManager	 *ptr_tex_manager_;

	// fbx sdk version
	FbxSdkVersion	fbx_sdk_version_;

	//fbx base date
	vector<FStaticMesh *> v_static_mesh_;
	vector<string> v_tex_name_;
	//vector<ID3D11ShaderResourceView *> v_tex_;

	FStaticMesh *ptr_cur_mesh_;
	FbxRawMeshVertexFormat *ptr_raw_mesh_;

};

