#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "Animator.h"

#include "Resources.h"
#include "ParticleSystem.h"
#include "Terrain.h"
#include "SphereCollider.h"
#include "MeshData.h"


#include "DXManager.h"
#include "BufferManager.h"


#include "PlayerScript.h"
#include "CameraHolder.h"


void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FinalUpdate();

}

// TEMP
void SceneManager::Render()
{
	if (_activeScene)
		_activeScene->Render();
}

void SceneManager::LoadScene(wstring sceneName)
{
	// TODO : ���� Scene ����
	// TODO : ���Ͽ��� Scene ���� �ε�

	_activeScene = LoadTestScene();

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::LoadData(wstring path, shared_ptr<class MeshData> data)
{
}


void SceneManager::SetLayerName(uint8 index, const wstring& name)
{
	// ���� ������ ����
	const wstring& prevName = _layerNames[index];
	_layerIndex.erase(prevName);

	_layerNames[index] = name;
	_layerIndex[name] = index;
}

uint8 SceneManager::LayerNameToIndex(const wstring& name)
{
	auto findIt = _layerIndex.find(name);
	if (findIt == _layerIndex.end())
		return 0;

	return findIt->second;
}



shared_ptr<GameObject> SceneManager::Pick(int32 screenX, int32 screenY)
{
	shared_ptr<Camera> camera = GetActiveScene()->GetMainCamera();

	float width = static_cast<float>(GEngine->GetWinfo().width);
	float height = static_cast<float>(GEngine->GetWinfo().height);

	Matrix projectionMatrix = camera->GetProjectionMatrix();

	// ViewSpace���� Picking ����
	float viewX = (+2.0f * screenX / width - 1.0f) / projectionMatrix(0, 0);
	float viewY = (-2.0f * screenY / height + 1.0f) / projectionMatrix(1, 1);

	Matrix viewMatrix = camera->GetViewMatrix();
	Matrix viewMatrixInv = viewMatrix.Invert();

	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

	float minDistance = FLT_MAX;
	shared_ptr<GameObject> picked;

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetCollider() == nullptr)
			continue;

		// ViewSpace������ Ray ����
		Vec4 rayOrigin = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		Vec4 rayDir = Vec4(viewX, viewY, 1.0f, 0.0f);

		// WorldSpace������ Ray ����
		rayOrigin = XMVector3TransformCoord(rayOrigin, viewMatrixInv);
		rayDir = XMVector3TransformNormal(rayDir, viewMatrixInv);
		rayDir.Normalize();

		// WorldSpace���� ����
		float distance = 0.f;
		if (gameObject->GetCollider()->Intersects(rayOrigin, rayDir, OUT distance) == false)
			continue;

		if (distance < minDistance)
		{
			minDistance = distance;
			picked = gameObject;
		}
	}

	return picked;
}
shared_ptr<Scene> SceneManager::LoadTestScene()
{
#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
#pragma endregion
	shared_ptr<Scene> scene = make_shared<Scene>();
	GET_SINGLE(Resources)->LoadRectangleMesh();
#pragma region ComputeShader
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"ComputeShader");

		//UAV �� Texture ����
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->CreateTexture(L"UAVTexture",
			DXGI_FORMAT_R8G8B8A8_UNORM, 1024, 1024, D3D11_BIND_UNORDERED_ACCESS, D3D11_USAGE::D3D11_USAGE_DEFAULT);

		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"ComputeShader");
		material->SetShader(shader);
		material->SetInt(0, 1);
		material->SetTexture(0, texture);

		auto uav = texture->GetUAV();
		material->GetShader()->Update();
		GET_SINGLE(BufferManager)->PushComputeUAV(UAV_REGISTER::u0, uav);

		// ������ �׷� (1 * 1024 * 1)
		material->Dispatch(1, 1024, 1);
	}
#pragma endregion
	

	///////////////////�÷��̾� OBJ ����/////////////////////
	///////////////////�÷��̾� OBJ ����/////////////////////
	///////////////////�÷��̾� OBJ ����/////////////////////


	shared_ptr<GameObject> MainCamera = nullptr;


#pragma region Camera
	{
		MainCamera = make_shared<GameObject>();
		MainCamera->SetName(L"Main_Camera");
		MainCamera->AddComponent(make_shared<Transform>());
		MainCamera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45��
		MainCamera->GetCamera()->SetFar(10000.f);
		MainCamera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, -800.f)); //�÷��̾�� �󸶳� ��������
		MainCamera->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		MainCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI�� �� ����
		scene->AddGameObject(MainCamera);
	}
#pragma endregion



#pragma region Player

	shared_ptr<GameObject> player = make_shared<GameObject>();
	player->SetName(L"Player");
	player->AddComponent(make_shared<Transform>());
	player->AddComponent(make_shared<PlayerScript>());
	player->SetCheckFrustum(false);
	player->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
	player->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	player->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));

	scene->AddGameObject(player);

#pragma endregion


#pragma region CameraHolder

	shared_ptr<GameObject> cameraHolder = make_shared<GameObject>();
	cameraHolder->SetName(L"Camera_Holder");
	cameraHolder->AddComponent(make_shared<Transform>());
	cameraHolder->GetTransform()->SetLocalPosition(Vec3(0.f, 50.f, 0.f)); 
	cameraHolder->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
	cameraHolder->AddComponent(make_shared<CameraHolder>());

#pragma endregion


#pragma region MeshHolder

	shared_ptr<GameObject> meshHolder = make_shared<GameObject>();
	meshHolder->SetName(L"MeshHolder");
	meshHolder->AddComponent(make_shared<Transform>());
	meshHolder->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f)); //�÷��̾�� �󸶳� ��������
	meshHolder->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));

#pragma endregion


	///////////////////�÷��̾� OBJ ��/////////////////////
	///////////////////�÷��̾� OBJ ��/////////////////////
	///////////////////�÷��̾� OBJ ��/////////////////////
#pragma region UI_Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Orthographic_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, 800*600
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskAll(); // �� ����
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI�� ����
		scene->AddGameObject(camera);
	}
#pragma endregion

#pragma region SkyBox
	{
		shared_ptr<GameObject> skybox = make_shared<GameObject>();
		skybox->AddComponent(make_shared<Transform>());
		skybox->SetCheckFrustum(false);
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Skybox");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Sky01", L"..\\Resources\\Texture\\Sky02.jpg");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		scene->AddGameObject(skybox);
	}
#pragma endregion

#pragma region Object
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"OBJ");
		obj->AddComponent(make_shared<Transform>());
		obj->AddComponent(make_shared<SphereCollider>());
		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		obj->GetTransform()->SetLocalPosition(Vec3(100, 0, 0));
		obj->SetStatic(false);
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
			meshRenderer->SetMaterial(material->Clone());
		}
		dynamic_pointer_cast<SphereCollider>(obj->GetCollider())->SetRadius(0.5f);
		dynamic_pointer_cast<SphereCollider>(obj->GetCollider())->SetCenter(Vec3(0.f, 0.f, 0.f));
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);
	}
#pragma endregion

#pragma region Object
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"OBJ");
		obj->AddComponent(make_shared<Transform>());
		//obj->AddComponent(make_shared<SphereCollider>());
		obj->GetTransform()->SetLocalScale(Vec3(1000.f, 1000.f, 10.f));
		obj->GetTransform()->SetLocalRotation(Vec3(90.f, 0.f, 0.f));
		obj->GetTransform()->SetLocalPosition(Vec3(0, 0, 0));
		obj->SetStatic(false);
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
			meshRenderer->SetMaterial(material->Clone());
		}
		//dynamic_pointer_cast<SphereCollider>(obj->GetCollider())->SetRadius(0.5f);
		//dynamic_pointer_cast<SphereCollider>(obj->GetCollider())->SetCenter(Vec3(0.f, 0.f, 0.f));
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);
	}
#pragma endregion

#pragma region Terrain
	//{
	//	shared_ptr<GameObject> obj = make_shared<GameObject>();
	//	obj->AddComponent(make_shared<Transform>());
	//	obj->AddComponent(make_shared<Terrain>());
	//	obj->AddComponent(make_shared<MeshRenderer>());

	//	obj->GetTransform()->SetLocalScale(Vec3(50.f, 250.f, 50.f));
	//	obj->GetTransform()->SetLocalPosition(Vec3(-100.f, -200.f, 300.f));
	//	obj->SetStatic(true);
	//	obj->GetTerrain()->Init(64, 64);
	//	obj->SetCheckFrustum(false);

	//	scene->AddGameObject(obj);
	//}
#pragma endregion

//#pragma region UI_Test
//	for (int32 i = 0; i < 6; i++)
//	{
//		shared_ptr<GameObject> obj = make_shared<GameObject>();
//		obj->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
//		obj->AddComponent(make_shared<Transform>());
//		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
//		obj->GetTransform()->SetLocalPosition(Vec3(-350.f + (i * 120), 250.f, 500.f));
//		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//		{
//			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
//			meshRenderer->SetMesh(mesh);
//		}
//		{
//			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
//
//			shared_ptr<Texture> texture;
//			if (i < 3)
//				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
//			else if (i < 5)
//				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
//			else
//				texture = GET_SINGLE(Resources)->Get<Texture>(L"UAVTexture");
//			//texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(0);
//
//			shared_ptr<Material> material = make_shared<Material>();
//			material->SetShader(shader);
//			material->SetTexture(0, texture);
//			meshRenderer->SetMaterial(material);
//		}
//		obj->AddComponent(meshRenderer);
//		scene->AddGameObject(obj);
//	}
//#pragma endregion

#pragma region Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0, 1000, 100));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0, -1, 1.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));

		scene->AddGameObject(light);
	}
#pragma endregion



#pragma region FBX
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Blader\\Man_Blader_Idle01.fbx");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		auto MainObject = gameObjects[0];
		auto vecAnimClips = MainObject->GetAnimator()->GetAnimClips();


		shared_ptr<MeshData> OtherData = GET_SINGLE(Resources)->LoadFBXAnimData(L"..\\Resources\\FBX\\Blader\\Man_Blader_Idle02.fbx");
		OtherData->AddAnimTake(gameObjects);
		OtherData = GET_SINGLE(Resources)->LoadFBXAnimData(L"..\\Resources\\FBX\\Blader\\Man_Blader_WeaponIn.fbx");
		OtherData->AddAnimTake(gameObjects);
		OtherData = GET_SINGLE(Resources)->LoadFBXAnimData(L"..\\Resources\\FBX\\Blader\\Man_Blader_Run01.fbx");
		OtherData->AddAnimTake(gameObjects);
		OtherData = GET_SINGLE(Resources)->LoadFBXAnimData(L"..\\Resources\\FBX\\Blader\\Man_Blader_Run02.fbx");
		OtherData->AddAnimTake(gameObjects);
		OtherData = GET_SINGLE(Resources)->LoadFBXAnimData(L"..\\Resources\\FBX\\Blader\\Man_Blader_NomalAttack01.fbx");
		OtherData->AddAnimTake(gameObjects);
		OtherData = GET_SINGLE(Resources)->LoadFBXAnimData(L"..\\Resources\\FBX\\Blader\\Man_Blader_NomalAttack02.fbx");
		OtherData->AddAnimTake(gameObjects);
		OtherData = GET_SINGLE(Resources)->LoadFBXAnimData(L"..\\Resources\\FBX\\Blader\\Man_Blader_NomalAttack03.fbx");
		OtherData->AddAnimTake(gameObjects);
		OtherData = GET_SINGLE(Resources)->LoadFBXAnimData(L"..\\Resources\\FBX\\Blader\\Man_Blader_Damage01.fbx");
		OtherData->AddAnimTake(gameObjects);
		OtherData = GET_SINGLE(Resources)->LoadFBXAnimData(L"..\\Resources\\FBX\\Blader\\Man_Blader_Damage02.fbx");
		OtherData->AddAnimTake(gameObjects);





		
		player->AddChildObject(cameraHolder);
		cameraHolder->AddChildObject(MainCamera);
		player->AddChildObject(meshHolder);
	
	

		for (auto& gameObject : gameObjects)
		{

			meshHolder->AddChildObject(gameObject);
			gameObject->SetName(L"Player");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 180.f, 0.f));
			scene->AddGameObject(gameObject);

		}
	}
#pragma endregion

	return scene;
}