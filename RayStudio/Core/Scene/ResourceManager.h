#pragma once
#include "FbxLoader.h"
#include "Level.h"
#include "../../Editor/Common/DeviceManager.h"
#include "../DXCommon.h"

#include <vector>

using std::vector;


struct EntityMeshBuffer
{
	ID3D11Buffer *ptr_vb_;
	ID3D11Buffer *ptr_ib_;

	uint16 section_count_;
	//vector<MeshSectionDesc*> v_section_desc_;
};

class ResourceManager
{
public:
	FbxLoader Loader;
	DeviceManager *pDeviceStream;
	Level *pLevel;

	vector<EntityMeshBuffer*> v_entity_buffer;
public:
	ResourceManager();
	ResourceManager(DeviceManager *pDeviceManager, Level *pSceneLevel);
	~ResourceManager();
	
	void Init(DeviceManager *pDeviceManager, Level *pSceneLevel);

	bool LoadResource(const string *filePath);

	vector<EntityMeshBuffer*> ResourceToEntity(vector<FStaticMesh *> StaticMesh);
	vector<Actor*> EntityToActor(vector<EntityMeshBuffer*> vEmp);
};

