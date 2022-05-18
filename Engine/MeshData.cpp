#include "pch.h"
#include "MeshData.h"
#include "FBXLoader.h"
#include "Mesh.h"
#include "Material.h"
#include "Resources.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Animator.h"

MeshData::MeshData() : Object(OBJECT_TYPE::MESH_DATA)
{
}

MeshData::~MeshData()
{
}

shared_ptr<MeshData> MeshData::LoadFromFBX(const wstring& path)
{
	FBXLoader loader;
	loader.LoadFbx(path);

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

shared_ptr<MeshData> MeshData::LoadFromFBXAnim(const wstring& path)
{
	FBXLoader loader;
	loader.LoadFbxAnim(path);

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromAnimFBX(&loader.GetMesh(i), loader);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		//// Material 찾아서 연동
		//vector<shared_ptr<Material>> materials;
		//for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		//{
		//	shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
		//	materials.push_back(material);
		//}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		//info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

void MeshData::AddAnimTake(vector<shared_ptr<GameObject>>& vecObject)
{
	

	int i = 0;
	for (MeshRenderInfo& info : _meshRenders)
	{
		
		if (info.mesh->IsAnimMesh())
		{

			auto _Animator = vecObject[i]->GetAnimator();
			auto MainMesh = _Animator->GetMeshRenderer()->GetMesh();
			auto Animclip = MainMesh->GetAnimClip();


			auto pAnimTakeVec = (info.mesh->GetAnimClip());
			auto AnimTake = (*pAnimTakeVec)[0];
			//vecAnimClipInfo->push_back(AnimTake);

			Animclip->push_back(AnimTake);
			MainMesh->AddBoneFrameBuffer(info.mesh->GetBoneFrameDataBuffer(0));

			i++;
		}
	
	}



}

void MeshData::Load(const wstring& _strFilePath)
{
	// TODO
}

void MeshData::Save(const wstring& _strFilePath)
{
	// TODO
}

vector<shared_ptr<GameObject>> MeshData::Instantiate()
{
	vector<shared_ptr<GameObject>> v;

	for (MeshRenderInfo& info : _meshRenders)
	{
		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->AddComponent(make_shared<Transform>());
		gameObject->AddComponent(make_shared<MeshRenderer>());
		gameObject->GetMeshRenderer()->SetMesh(info.mesh);

		for (uint32 i = 0; i < info.materials.size(); i++)
			gameObject->GetMeshRenderer()->SetMaterial(info.materials[i], i);

		if (info.mesh->IsAnimMesh())
		{
			shared_ptr<Animator> animator = make_shared<Animator>();
			gameObject->AddComponent(animator);
			animator->SetBones(info.mesh->GetBones());
			animator->SetAnimClip(info.mesh->GetAnimClip());


		/*	auto pAnimTakeVec = (info.mesh->GetAnimClip());
			auto AnimTake = (*pAnimTakeVec)[0];
			pAnimTakeVec->push_back(AnimTake);*/
			

		}

		v.push_back(gameObject);
	}


	return v;
}

