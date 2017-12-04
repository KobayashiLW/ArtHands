#include "FbxLoader.h"


FbxLoader::FbxLoader()
{

}

FbxLoader::FbxLoader(TextureManager *pTextureManager)
{
}


FbxLoader::~FbxLoader()
{
}


bool FbxLoader::Initialize()
{
	if (ptr_fbx_manager_)
	{
		ptr_fbx_manager_->Destroy();
	}
	ptr_fbx_manager_ = FbxManager::Create();

	if (ptr_fbx_manager_ == NULL)
	{
		return false;
	}

	// Create an IOSettings object  
	FbxIOSettings* ios = FbxIOSettings::Create(ptr_fbx_manager_, IOSROOT);
	ptr_fbx_manager_->SetIOSettings(ios);

	// Load plug-ins from the executable directory  
	string lExtension = "dll";
	string  lPath = FbxGetApplicationDirectory();
	ptr_fbx_manager_->LoadPluginsDirectory(lPath.c_str(), lExtension.c_str());

	// Create the entity that hold the whole Scene  
	ptr_fbx_scene_ = FbxScene::Create(ptr_fbx_manager_, "");

	return true;
}

bool FbxLoader::LoadScene(const string *pFilePath)
{
	if (ptr_fbx_manager_ == NULL)
	{
		return false;
	}

	// Get the file version number generate by the FBX SDK.  
	FbxManager::GetFileFormatVersion(fbx_sdk_version_.mMajor, fbx_sdk_version_.mMinor, fbx_sdk_version_.mRevision);

	// Create an importer.  
	FbxImporter* pKFBXImporter = FbxImporter::Create(ptr_fbx_manager_, "");

	//char buffer[100] = { 0 };
	//WcharToChar(pFilePath->c_str(), buffer);
	bool importStatus = pKFBXImporter->Initialize(pFilePath->c_str(), -1, ptr_fbx_manager_->GetIOSettings());

	if (!importStatus)
	{
		return false;
	}

	// Import the scene  
	ptr_fbx_scene_->Clear();

	importStatus = pKFBXImporter->Import(ptr_fbx_scene_);

	// Destroy the importer.  
	pKFBXImporter->Destroy();

	return importStatus;
}


vector<FStaticMesh *> FbxLoader::ReadMeshFormFbx(const string *pFilePath)
{
	bool hr = LoadScene(pFilePath);
	if (!hr)
	{
		return v_static_mesh_;
	}

	FbxNode *pRootNode = ptr_fbx_scene_->GetRootNode();
	ProcessNode(pRootNode);

	size_t MeshCount = v_static_mesh_.size();
	return v_static_mesh_;
}

void FbxLoader::ProcessNode(FbxNode* pNode)
{
	int nodeCount = pNode->GetChildCount();
	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		FbxNode *pNodeIndex = pNode->GetChild(i);
		if (pNodeIndex->GetNodeAttribute())
		{
			switch (pNodeIndex->GetNodeAttribute()->GetAttributeType())
			{
			case FbxNodeAttribute::eMesh:
				ProcessMesh(pNodeIndex);
				break;
			}
		}
	}
}

void FbxLoader::ProcessMesh(FbxNode* pNode)
{
	FbxMesh* pMesh = pNode->GetMesh();
	if (!pMesh)
		return;

	FbxMatrix globalMatrix = pMesh->GetNode()->EvaluateGlobalTransform();
	bool bIsTriMesh = pMesh->IsTriangleMesh();
	if (!bIsTriMesh)
	{

	}
	int ControlVertexCount = pMesh->GetControlPointsCount();//获得当前网格的控制点数
	int TriangleCount = pMesh->GetPolygonCount();  //获得当前网格的三角形数
	uint32_t RawVertexCount = TriangleCount * 3;

	bool bIsHasNormal = false;
	bool bIsHasTangent = false;

	//read texcoord date
	FbxLayerElementUV* pTexCoords = pMesh->GetLayer(0)->GetUVs();
	if (pTexCoords)
	{

	}
	//read normal date
	FbxLayerElementNormal *pNormal = pMesh->GetLayer(0)->GetNormals();
	if (pNormal)
	{
		bIsHasNormal = true;
	}
	else
	{
		// 自己计算法线数据
	}

	//read tangent date
	FbxLayerElementTangent* pTangent = pMesh->GetLayer(0)->GetTangents();
	if (pTangent)
	{
		bIsHasTangent = true;
	}
	else
	{
		// 自己计算切线数据
	}

	FbxRawVertex *pRawVertex = new FbxRawVertex[RawVertexCount];
	FVector *pCtrlVertex = new FVector[ControlVertexCount];

	int VertexCounter = 0;

	for (int i = 0; i < TriangleCount; i++)
	{
		int Index = i * 3;
		for (int j = 0; j < 3; j++)
		{
			int CtrlPointIndex = pMesh->GetPolygonVertex(i, j);

			pRawVertex[Index + j].VertexIndex = CtrlPointIndex;
			pRawVertex[Index + j].Vertex = pCtrlVertex[CtrlPointIndex];

			//read Texcoords data
			ReadUV(pTexCoords, CtrlPointIndex, pMesh->GetTextureUVIndex(i, j), pRawVertex[Index + j]);
			//read normal data
			if (bIsHasNormal)
			{
				ReadNormal(pNormal, CtrlPointIndex, VertexCounter, pRawVertex[Index + j]);
			}
			else
			{

			}
			//read tangent data
			if (bIsHasTangent)
			{
				ReadTangent(pTangent, CtrlPointIndex, VertexCounter, pRawVertex[Index + j]);
			}
			else
			{

			}

			pRawVertex[Index + j].Mask = false;
			VertexCounter++;
		}
	}
	safe_delete(pCtrlVertex)

	int SortIndex = 0;
	//合并顶点，由于存在重复的顶点，通过比较他们的顶点索引来确定他们是否重复
	for (uint32_t i = 0; i < RawVertexCount; i++)
	{
		if (pRawVertex[i].Mask)//顶点是否为重复数据
			continue;
		for (uint32_t j = i + 1; j < RawVertexCount; j++)
		{
			if (pRawVertex[i].VertexIndex != pRawVertex[j].VertexIndex) //只具有相同的控制点索引的顶点才有可能其他属性数据相等
				continue;
			if (pRawVertex[j].Mask) //顶点是否为重复数据
				continue;
			if (pRawVertex[i].TexcoordIndex == pRawVertex[j].TexcoordIndex || (pRawVertex[i].Texcoord == pRawVertex[j].Texcoord))
			{
				if (pRawVertex[i].NoramlIndex == pRawVertex[j].NoramlIndex || (pRawVertex[i].Normal == pRawVertex[j].Normal))
				{
					if (pRawVertex[i].TangentIndex == pRawVertex[j].TangentIndex || (pRawVertex[i].Tangent == pRawVertex[j].Tangent))
					{
						pRawVertex[j].Mask = true;
						pRawVertex[j].PointToVertexIndex = SortIndex;
					}
				}
			}
		}
		pRawVertex[i].PointToVertexIndex = SortIndex;
		SortIndex++;
	}
	FStaticMeshVertex *pStaticMeshVertex = new FStaticMeshVertex[SortIndex];
	for (uint32_t i = 0, j = 0; i < RawVertexCount; i++)
	{
		if (pRawVertex[i].Mask == false)
		{
			pStaticMeshVertex[j].Vertex = pRawVertex[i].Vertex;
			pStaticMeshVertex[j].Normal = pRawVertex[i].Normal;
			pStaticMeshVertex[j].Tangent = pRawVertex[i].Tangent;
			pStaticMeshVertex[j++].Texcoord = pRawVertex[i].Texcoord;
		}
	}

	ptr_cur_mesh_ = new FStaticMesh();
	ptr_cur_mesh_->ptr_vertex_ = pStaticMeshVertex;
	ptr_cur_mesh_->vertex_count_ = SortIndex;

	//read mesh material
	ReadMaterial(pMesh);

	//重新计算子网格的三角形顶点索引
	uint16 SectionCount = ptr_cur_mesh_->section_count_;
	for (uint16 i = 0, Index = 0; i < SectionCount; i++)
	{
		size_t PolygenCount = ptr_cur_mesh_->v_mesh_section_[i].v_polygen_index_list_.size();
		size_t VertexCount = PolygenCount * 3;
		//ptr_cur_mesh_->v_mesh_section_[i].v_vertex_index_list_.reserve(VertexCount);
		for (size_t j = 0; j < PolygenCount; j++)
		{
			uint16 PolygenIndex = ptr_cur_mesh_->v_mesh_section_[i].v_polygen_index_list_[j];
			uint32 VertexIndex = PolygenIndex * 3;
			for (int k = 0; k < 3; k++)
			{
				ptr_cur_mesh_->v_mesh_section_[i].v_vertex_index_list_.push_back(pRawVertex[VertexIndex + k].PointToVertexIndex);
			}
		}
	}
	safe_delete_array(pRawVertex);
	for (int i = 0; i < ptr_cur_mesh_->section_count_; i++)
	{
		size_t SectionPolygenIndexCount = ptr_cur_mesh_->v_mesh_section_[i].v_polygen_index_list_.size();
		size_t SectionVertexIndexCount = ptr_cur_mesh_->v_mesh_section_[i].v_vertex_index_list_.size();

		ptr_cur_mesh_->v_mesh_section_[i].ptr_polygen_index_ = new uint16[SectionPolygenIndexCount];
		ptr_cur_mesh_->v_mesh_section_[i].ptr_vertex_index_ = new uint16[SectionVertexIndexCount];

		memcpy(ptr_cur_mesh_->v_mesh_section_[i].ptr_polygen_index_, &ptr_cur_mesh_->v_mesh_section_[i].v_polygen_index_list_[0], SectionPolygenIndexCount * sizeof(uint16));
		memcpy(ptr_cur_mesh_->v_mesh_section_[i].ptr_vertex_index_, &ptr_cur_mesh_->v_mesh_section_[i].v_vertex_index_list_[0], SectionVertexIndexCount * sizeof(uint16));
	

		ptr_cur_mesh_->v_mesh_section_[i].polygen_index_count_ = SectionPolygenIndexCount;
		ptr_cur_mesh_->v_mesh_section_[i].vertex_index_count_ = SectionVertexIndexCount;

		ptr_cur_mesh_->v_mesh_section_[i].v_polygen_index_list_.clear();
		ptr_cur_mesh_->v_mesh_section_[i].v_vertex_index_list_.clear();


	}



	v_static_mesh_.push_back(ptr_cur_mesh_);
}

bool FbxLoader::ReadMaterial(FbxMesh* pMesh)
{
	int MaterialCount = 0;
	FbxNode* pNode = NULL;

	if (pMesh && pMesh->GetNode())
	{
		pNode = pMesh->GetNode();
		MaterialCount = pNode->GetMaterialCount();
	}
	else
	{
		return false;
	}
	ptr_cur_mesh_->section_count_ = MaterialCount;
	ptr_cur_mesh_->v_mesh_section_.resize(MaterialCount);

	if (MaterialCount)
	{
		for (int i = 0; i < MaterialCount; i++)
		{

			FbxSurfaceMaterial* pSurfaceMaterial = pNode->GetMaterial(i);
			FMaterial Material;
			ReadMaterialAttribute(pSurfaceMaterial, Material);
			ptr_cur_mesh_->v_mesh_material_.push_back(Material);
		}

		//read sub mesh index and material date 

		FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
		lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
		const int MaterialIndiceCount = lMaterialIndice->GetCount();

		const int triangleCount = pMesh->GetPolygonCount();
		const int controlPointCount = pMesh->GetControlPointsCount();

		////求材质索引最大值
		//int maxMaterialIndex = -1;
		//for (int i = 0; i < MaterialIndiceCount; i++)
		//{
		//	int curMaterialIndex = lMaterialIndice->GetAt(i);
		//	if (curMaterialIndex > maxMaterialIndex)
		//		maxMaterialIndex = curMaterialIndex;
		//}//得到maxMaterialIndex

		for (int i = 0; i < triangleCount; i++)
		{
			int lMaterialIndex;//当前多边形材质索引
			if (i < MaterialIndiceCount) //如果i对lMaterialIndice不越界
				lMaterialIndex = lMaterialIndice->GetAt(i);
			else
				lMaterialIndex = 0;
			ptr_cur_mesh_->v_mesh_section_[lMaterialIndex].v_polygen_index_list_.push_back(i);
		}
	}


	return true;
}

bool FbxLoader::ReadMaterialAttribute(FbxSurfaceMaterial* pSurfaceMaterial, FMaterial &Material)
{

	// Get the name of material  
	const char *materialName = pSurfaceMaterial->GetName();

	// Phong material  
	if (pSurfaceMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		FbxDouble3 Ambient = ((FbxSurfacePhong*)pSurfaceMaterial)->Ambient;
		FbxDouble3 Diffuse = ((FbxSurfacePhong*)pSurfaceMaterial)->Diffuse;
		FbxDouble3 Specular = ((FbxSurfacePhong*)pSurfaceMaterial)->Specular;
		FbxDouble3 Emissive = ((FbxSurfacePhong*)pSurfaceMaterial)->Emissive;
		FbxDouble Shininess = ((FbxSurfacePhong*)pSurfaceMaterial)->Shininess;

		Material.Ambient = FVector4((float)Ambient.mData[0], (float)Ambient.mData[1], (float)Ambient.mData[2], 1);
		Material.Diffuse = FVector4((float)Diffuse.mData[0], (float)Diffuse.mData[1], (float)Diffuse.mData[2], 1);
		Material.Specular = FVector4((float)Specular.mData[0], (float)Specular.mData[1], (float)Specular.mData[2], Shininess);
		Material.Emissive = FVector4((float)Emissive.mData[0], (float)Emissive.mData[1], (float)Emissive.mData[2], 1);

	}

	// Lambert material  
	if (pSurfaceMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		FbxDouble3 Ambient = ((FbxSurfaceLambert*)pSurfaceMaterial)->Ambient;
		FbxDouble3 Diffuse = ((FbxSurfaceLambert*)pSurfaceMaterial)->Diffuse;
		FbxDouble3 Emissive = ((FbxSurfaceLambert*)pSurfaceMaterial)->Emissive;
		FbxDouble factor = ((FbxSurfaceLambert*)pSurfaceMaterial)->TransparencyFactor;

		Material.Ambient = FVector4((float)Ambient.mData[0], (float)Ambient.mData[1], (float)Ambient.mData[2], 1);
		Material.Diffuse = FVector4((float)Diffuse.mData[0], (float)Diffuse.mData[1], (float)Diffuse.mData[2], 1);
		Material.Emissive = FVector4((float)Emissive.mData[0], (float)Emissive.mData[1], (float)Emissive.mData[2], 1);
	}

	const char * pPropertyName = FbxSurfaceMaterial::sDiffuse;
	const char * pPropertyNormalMapName = FbxSurfaceMaterial::sNormalMap;
	const FbxProperty lPropertyDiffuse = pSurfaceMaterial->FindProperty(pPropertyName);
	const FbxProperty lPropertyNormalMap = pSurfaceMaterial->FindProperty(pPropertyNormalMapName);

	if (lPropertyDiffuse.IsValid())
	{
		const int lTextureCount = lPropertyDiffuse.GetSrcObjectCount<FbxFileTexture>();
		if (lTextureCount)
		{
			const FbxFileTexture* lTexture = lPropertyDiffuse.GetSrcObject<FbxFileTexture>();
			const char* tfilename = lTexture->GetFileName();
			//	ID3D11ShaderResourceView *pTex = LoadMaterialTexture(tfilename);
			//if (pTex)
			//	(*ppMaterial)->diffuse_tex_ = pTex;
			//else
			//	(*ppMaterial)->diffuse_tex_ = NULL;

		}
	}
	if (lPropertyNormalMap.IsValid())
	{
		const int lTextureCount = lPropertyNormalMap.GetSrcObjectCount<FbxFileTexture>();
		if (lTextureCount)
		{
			const FbxFileTexture* lTexture = lPropertyNormalMap.GetSrcObject<FbxFileTexture>();
			const char* tfilename = lTexture->GetFileName();
		/*	ID3D11ShaderResourceView *pTex = LoadMaterialTexture(tfilename);
			if (pTex)
				(*ppMaterial)->normal_tex_ = pTex;
			else
				(*ppMaterial)->normal_tex_ = NULL;*/
		}
	}

	return true;
}


inline void FbxLoader::ReadUV(FbxLayerElementUV* pTexCoords, int CtrlPointIndex, int TexUVIndex, FbxRawVertex &RawVertex)
{
	switch (pTexCoords->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		switch (pTexCoords->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			RawVertex.TexcoordIndex = CtrlPointIndex;
			RawVertex.Texcoord = FVector2D(pTexCoords->GetDirectArray().GetAt(CtrlPointIndex).mData[0], pTexCoords->GetDirectArray().GetAt(CtrlPointIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int ID = pTexCoords->GetIndexArray().GetAt(CtrlPointIndex);

			RawVertex.TexcoordIndex = ID;
			RawVertex.Texcoord = FVector2D(pTexCoords->GetDirectArray().GetAt(ID).mData[0], pTexCoords->GetDirectArray().GetAt(ID).mData[1]);
		}
		break;

		default:
			break;
		}
	}
	break;

	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (pTexCoords->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			RawVertex.TexcoordIndex = TexUVIndex;
			RawVertex.Texcoord = FVector2D(pTexCoords->GetDirectArray().GetAt(TexUVIndex).mData[0], pTexCoords->GetDirectArray().GetAt(TexUVIndex).mData[1]);
		}
		break;

		default:
			break;
		}
	}
	break;
	}

}

inline void FbxLoader::ReadNormal(FbxLayerElementNormal* pNormal, int CtrlPointIndex, int VertexCounter, FbxRawVertex &RawVertex)
{
	switch (pNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		switch (pNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			RawVertex.NoramlIndex = CtrlPointIndex;
			RawVertex.Normal = FVector(pNormal->GetDirectArray().GetAt(CtrlPointIndex).mData[0], pNormal->GetDirectArray().GetAt(CtrlPointIndex).mData[1], pNormal->GetDirectArray().GetAt(CtrlPointIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int ID = pNormal->GetIndexArray().GetAt(CtrlPointIndex);

			RawVertex.NoramlIndex = ID;
			RawVertex.Normal = FVector(pNormal->GetDirectArray().GetAt(ID).mData[0], pNormal->GetDirectArray().GetAt(ID).mData[1], pNormal->GetDirectArray().GetAt(ID).mData[2]);
		}
		break;

		default:
			break;
		}
	}
	break;

	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (pNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			RawVertex.NoramlIndex = VertexCounter;
			RawVertex.Normal = FVector(pNormal->GetDirectArray().GetAt(VertexCounter).mData[0], pNormal->GetDirectArray().GetAt(VertexCounter).mData[1], pNormal->GetDirectArray().GetAt(VertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int ID = pNormal->GetIndexArray().GetAt(VertexCounter);

			RawVertex.NoramlIndex = ID;
			RawVertex.Normal = FVector(pNormal->GetDirectArray().GetAt(ID).mData[0], pNormal->GetDirectArray().GetAt(ID).mData[1], pNormal->GetDirectArray().GetAt(ID).mData[2]);
		}
		break;

		default:
			break;
		}
	}
	break;
	}
}

inline void FbxLoader::ReadTangent(FbxLayerElementTangent* pTangent, int CtrlPointIndex, int VertexCounter, FbxRawVertex &RawVertex)
{
	switch (pTangent->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		switch (pTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			RawVertex.TangentIndex = CtrlPointIndex;
			RawVertex.Tangent = FVector(pTangent->GetDirectArray().GetAt(CtrlPointIndex).mData[0], pTangent->GetDirectArray().GetAt(CtrlPointIndex).mData[1], pTangent->GetDirectArray().GetAt(CtrlPointIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int ID = pTangent->GetIndexArray().GetAt(CtrlPointIndex);

			RawVertex.TangentIndex = ID;
			RawVertex.Tangent = FVector(pTangent->GetDirectArray().GetAt(ID).mData[0], pTangent->GetDirectArray().GetAt(ID).mData[1], pTangent->GetDirectArray().GetAt(ID).mData[2]);
		}
		break;

		default:
			break;
		}
	}
	break;

	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (pTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			RawVertex.TangentIndex = VertexCounter;
			RawVertex.Tangent = FVector(pTangent->GetDirectArray().GetAt(VertexCounter).mData[0], pTangent->GetDirectArray().GetAt(VertexCounter).mData[1], pTangent->GetDirectArray().GetAt(VertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int ID = pTangent->GetIndexArray().GetAt(VertexCounter);

			RawVertex.TangentIndex = ID;
			RawVertex.Tangent = FVector(pTangent->GetDirectArray().GetAt(ID).mData[0], pTangent->GetDirectArray().GetAt(ID).mData[1], pTangent->GetDirectArray().GetAt(ID).mData[2]);
		}
		break;

		default:
			break;
		}
	}
	break;
	}
}