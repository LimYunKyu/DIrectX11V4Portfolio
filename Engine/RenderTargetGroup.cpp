#include "pch.h"
#include "RenderTargetGroup.h"
#include "Engine.h"
#include "DXManager.h"


void RenderTargetGroup::Create(RENDER_TARGET_GROUP_TYPE groupType, vector<RenderTarget>& rtVec, shared_ptr<Texture> dsTexture)
{
	_groupType = groupType;
	_rtVec = rtVec;
	_rtCount = static_cast<uint32>(rtVec.size());
	_dsTexture = dsTexture;

	for (auto rt : rtVec)
	{
		_rtvVec.push_back(rt.target->GetRTV());
	}

	


}

void RenderTargetGroup::OMSetRenderTargets()
{

	D3D11_VIEWPORT _viewport = D3D11_VIEWPORT{0.f,0.f,_rtVec[0].target->GetWidth() , _rtVec[0].target->GetHeight(), 0.f, 1.f };
	D3D11_RECT rect = D3D11_RECT{ 0, 0, static_cast<LONG>(_rtVec[0].target->GetWidth()),  static_cast<LONG>(_rtVec[0].target->GetHeight()) };

	DEVICECONTEXT->RSSetViewports(1, &_viewport);
	DEVICECONTEXT->RSSetScissorRects(1, &rect);

	DEVICECONTEXT->OMSetRenderTargets(_rtCount, &_rtvVec[0], _dsTexture->GetDSV());
	
	
}


void RenderTargetGroup::ClearRenderTargetView()
{
	
	DEVICECONTEXT->ClearDepthStencilView(_dsTexture->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// Clear the back buffer 
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // red,green,blue,alpha
	/*if (_groupType == RENDER_TARGET_GROUP_TYPE::G_BUFFER)
	{
		ClearColor[0] = 0.5f;
		ClearColor[1] = 0.0f;
		ClearColor[2] = 0.0f;
		ClearColor[3] = 1.0f;
	}
	else if (_groupType == RENDER_TARGET_GROUP_TYPE::LIGHTING)
	{
		ClearColor[0] = 0.0f;
		ClearColor[1] = 0.5f;
		ClearColor[2] = 0.0f;
		ClearColor[3] = 1.0f;
	}
	else if (_groupType == RENDER_TARGET_GROUP_TYPE::SHADOW)
	{
		ClearColor[0] = 0.0f;
		ClearColor[1] = 0.0f;
		ClearColor[2] = 0.5f;
		ClearColor[3] = 1.0f;
	}*/
	
	

	for (uint32 i = 0; i < _rtCount; i++)
	{
		DEVICECONTEXT->ClearRenderTargetView(_rtvVec[i], ClearColor);
	}

	
}

