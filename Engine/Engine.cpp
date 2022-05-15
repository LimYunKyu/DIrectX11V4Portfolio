#include "pch.h"
#include "Engine.h"
#include "DXManager.h"
#include "BufferManager.h"
#include "Light.h"
#include "Material.h"
#include "Input.h"
#include "Timer.h"
#include "Resources.h"
#include "SceneManager.h"
#include "InstancingManager.h"

void Engine::Init(WindowInfo _info)
{
	mWinfo = _info;

	DXManager::GetInstance()->Init();
	BufferManager::GetInstance()->CreateConstantBuffer(CBV_REGISTER::b0, sizeof(LightParams));
	BufferManager::GetInstance()->CreateConstantBuffer(CBV_REGISTER::b1, sizeof(TransformParams));
	BufferManager::GetInstance()->CreateConstantBuffer(CBV_REGISTER::b2, sizeof(MaterialParams));

	CreateRenderTargetGroups();

	GET_SINGLE(Input)->Init(mWinfo.hwnd);
	GET_SINGLE(Timer)->Init();
	GET_SINGLE(Resources)->Init();
	GET_SINGLE(SceneManager)->LoadScene(L"TestScene");
}

void Engine::Update()
{
	
	GET_SINGLE(InstancingManager)->ClearBuffer();
	GET_SINGLE(Input)->Update();
	GET_SINGLE(Timer)->Update();
	GET_SINGLE(SceneManager)->Update();
	


	uint32 fps = GET_SINGLE(Timer)->GetFps();

	WCHAR text[100] = L"";
	::wsprintf(text, L"FPS : %d", fps);

	::SetWindowText(mWinfo.hwnd, text);

	Render();
}

void Engine::Render()
{
	
	GET_SINGLE(SceneManager)->Render();
    GET_SINGLE(DXManager)->RenderEnd();
	

}

void Engine::CreateRenderTargetGroups()
{
	// DepthStencil
	shared_ptr<Texture> dsTexture = GET_SINGLE(Resources)->CreateTexture(L"DepthStencil",
		DXGI_FORMAT_D32_FLOAT, mWinfo.width, mWinfo.height,	
		D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE::D3D11_USAGE_DEFAULT);

	// SwapChain Group
	{
		vector<RenderTarget> rtVec(SWAP_CHAIN_BUFFER_COUNT);

		for (uint32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
		{
			wstring name = L"SwapChainTarget_" + std::to_wstring(i);

			auto buffer = DXManager::GetInstance()->GeBacktBuffer();
			rtVec[i].target = GET_SINGLE(Resources)->CreateTextureFromResource(name, buffer);
		}

		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)] = make_shared<RenderTargetGroup>();
		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)]->Create(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN, rtVec, dsTexture);
	}

	// Shadow Group
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_SHADOW_GROUP_MEMBER_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture(L"ShadowTarget",
			DXGI_FORMAT_R32_FLOAT, 4096, 4096, D3D11_BIND_RENDER_TARGET, D3D11_USAGE::D3D11_USAGE_DEFAULT);

		shared_ptr<Texture> shadowDepthTexture = GET_SINGLE(Resources)->CreateTexture(L"ShadowDepthStencil",
			DXGI_FORMAT_D32_FLOAT, 4096, 4096,D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE::D3D11_USAGE_DEFAULT);

		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SHADOW)] = make_shared<RenderTargetGroup>();
		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SHADOW)]->Create(RENDER_TARGET_GROUP_TYPE::SHADOW, rtVec, shadowDepthTexture);
	}

	// Deferred Group
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture(L"PositionTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, mWinfo.width, mWinfo.height,
			D3D11_BIND_RENDER_TARGET, D3D11_USAGE::D3D11_USAGE_DEFAULT);

		rtVec[1].target = GET_SINGLE(Resources)->CreateTexture(L"NormalTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, mWinfo.width, mWinfo.height,
			D3D11_BIND_RENDER_TARGET, D3D11_USAGE::D3D11_USAGE_DEFAULT);

		rtVec[2].target = GET_SINGLE(Resources)->CreateTexture(L"DiffuseTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, mWinfo.width, mWinfo.height,
			D3D11_BIND_RENDER_TARGET, D3D11_USAGE::D3D11_USAGE_DEFAULT);

		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)] = make_shared<RenderTargetGroup>();
		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)]->Create(RENDER_TARGET_GROUP_TYPE::G_BUFFER, rtVec, dsTexture);
	}

	// Lighting Group
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture(L"DiffuseLightTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, mWinfo.width, mWinfo.height,
			D3D11_BIND_RENDER_TARGET, D3D11_USAGE::D3D11_USAGE_DEFAULT);

		rtVec[1].target = GET_SINGLE(Resources)->CreateTexture(L"SpecularLightTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, mWinfo.width, mWinfo.height,
			D3D11_BIND_RENDER_TARGET, D3D11_USAGE::D3D11_USAGE_DEFAULT);

		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::LIGHTING)] = make_shared<RenderTargetGroup>();
		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::LIGHTING)]->Create(RENDER_TARGET_GROUP_TYPE::LIGHTING, rtVec, dsTexture);
	}


}
