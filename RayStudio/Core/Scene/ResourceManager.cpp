#include "ResourceManager.h"
#include "StaticMeshActor.h"
#include <tchar.h>

ResourceManager::ResourceManager()
{
	Loader.Initialize();
}

ResourceManager::ResourceManager(DeviceManager *pDeviceManager, Level *pSceneLevel)
{
	pDeviceStream = pDeviceManager;
	pLevel = pSceneLevel;
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Init(DeviceManager *pDeviceManager, Level *pSceneLevel)
{
	pDeviceStream = pDeviceManager;
	pLevel = pSceneLevel;
}

bool ResourceManager::LoadResource(const string *filePath)
{
	vector<FStaticMesh *> StaticMeshs = Loader.ReadMeshFormFbx(filePath);
	vector<EntityMeshBuffer *> EntityBuffers = ResourceToEntity(StaticMeshs);
	vector<Actor *> Actors = EntityToActor(EntityBuffers);

	v_entity_buffer.insert(v_entity_buffer.end(), EntityBuffers.begin(), EntityBuffers.end());
	pLevel->Actors.insert(pLevel->Actors.end(), Actors.begin(), Actors.end());

	return true;
}


vector<EntityMeshBuffer*> ResourceManager::ResourceToEntity(vector<FStaticMesh *> StaticMesh)
{
	vector<EntityMeshBuffer*> vEmp;
	size_t MeshCount = StaticMesh.size();
	for (size_t MeshIndex = 0; MeshIndex < MeshCount; MeshIndex++)
	{
		//首先创建描述
		D3D11_BUFFER_DESC vbDesc = { 0 };
		vbDesc.ByteWidth = StaticMesh[MeshIndex]->vertex_count_ * sizeof(FStaticMeshVertex);
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA vbData = { 0 };
		vbData.pSysMem = StaticMesh[MeshIndex]->ptr_vertex_;
		EntityMeshBuffer *pEmp = new EntityMeshBuffer();
		//根描述和数据创建顶点缓存
		HRESULT hr = pDeviceStream->ptr_id3d_device_->CreateBuffer(&vbDesc, &vbData, &pEmp->ptr_vb_);
		if (FAILED(hr))
		{
			MessageBox(NULL, _T("CreateVertexBuffer错误!"), _T("错误"), MB_OK);
			return vEmp;
		}
		uint16 SectionCount = StaticMesh[MeshIndex]->section_count_;
		for (uint16 SectionIndex = 0; SectionIndex < SectionCount; SectionIndex++)
		{
			FStaticMeshSection &MeshSection = StaticMesh[MeshIndex]->v_mesh_section_[SectionIndex];
			//首先给出描述
			D3D11_BUFFER_DESC ibDesc = { 0 };
			ibDesc.ByteWidth = MeshSection.vertex_index_count_ * sizeof(uint16);
			ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			ibDesc.Usage = D3D11_USAGE_IMMUTABLE;
			D3D11_SUBRESOURCE_DATA ibData = { 0 };
			ibData.pSysMem = MeshSection.ptr_vertex_index_;
			//根据描述和数据创建索引缓存
			hr = pDeviceStream->ptr_id3d_device_->CreateBuffer(&ibDesc, &ibData, &pEmp->ptr_ib_);
			if (FAILED(hr))
			{
				MessageBox(NULL, _T("CreateIndexBuffer错误!"), _T("错误"), MB_OK);
				return vEmp;
			}
		}
		vEmp.push_back(pEmp);
	}

	return vEmp;
}

vector<Actor*> ResourceManager::EntityToActor(vector<EntityMeshBuffer*> vEmp)
{
	vector<Actor*> vActor;
	size_t EntityCount = vEmp.size();
	for (size_t Index = 0; Index < EntityCount; Index++)
	{
		StaticMeshActor *pActor = new StaticMeshActor();
		pActor->ptr_entity_ = vEmp[Index];

		vActor.push_back(pActor);
	}
	return vActor;
}