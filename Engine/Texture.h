#pragma once
#include "Object.h"

class Texture : public Object
{
public:
	Texture();
	virtual ~Texture();

	virtual void Load(const wstring& path) override;

public:
	void Create(DXGI_FORMAT format, uint32 width, uint32 height,
		D3D11_BIND_FLAG flags,D3D11_USAGE usage,Vec4 clearColor = Vec4(0.0f,0.f,0.f,0.f));

	void CreateFromResource(ID3D11Texture2D* tex2D);

public:
	ID3D11Texture2D* GetTex2D() { return _tex2D; }
	ID3D11ShaderResourceView* GetSRV() { return _srv; }
	ID3D11RenderTargetView* GetRTV() { return _rtv; }
	ID3D11DepthStencilView* GetDSV() { return _dsv; }
	ID3D11UnorderedAccessView* GetUAV() { return _uav; }


	float GetWidth() { return static_cast<float>(_desc.Width); }
	float GetHeight() { return static_cast<float>(_desc.Height); }

private:

	ID3D11Texture2D*				_tex2D;
	D3D11_TEXTURE2D_DESC			_desc;

	ScratchImage			 		_image;
	ID3D11DepthStencilView*			_dsv;
	ID3D11ShaderResourceView*		_srv;
	ID3D11RenderTargetView*			_rtv;
	ID3D11UnorderedAccessView*		_uav;
	



};

