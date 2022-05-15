#include "pch.h"
#include "Texture.h"
#include "Engine.h"
#include "DXManager.h"

Texture::Texture() : Object(OBJECT_TYPE::TEXTURE)
{

}

Texture::~Texture()
{

}

void Texture::Load(const wstring& path)
{


	wchar_t ext[_MAX_EXT] = {};
	_wsplitpath_s(path.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);

	ScratchImage image;
	HRESULT hr;
	if (_wcsicmp(ext, L".dds") == 0)
	{
		hr = LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, image);
	}
	else if (_wcsicmp(ext, L".tga") == 0)
	{
		hr = LoadFromTGAFile(path.c_str(), nullptr, image);
	}
	else if (_wcsicmp(ext, L".hdr") == 0)
	{
		hr = LoadFromHDRFile(path.c_str(), nullptr, image);
	}
	else
	{
		hr = LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, image);
	}



	if (SUCCEEDED(hr))
	{


		hr = CreateShaderResourceView(DEVICE,
			image.GetImages(), image.GetImageCount(),
			image.GetMetadata(), &(_srv));
	}





	//// 파일 확장자 얻기
	//wstring ext = fs::path(path).extension();

	//
	//if (ext == L".dds" || ext == L".DDS")
	//	::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, _image);
	//else if (ext == L".tga" || ext == L".TGA")
	//	::LoadFromTGAFile(path.c_str(), nullptr, _image);
	//else // png, jpg, jpeg, bmp
	//	::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, _image);



	//CreateShaderResourceView(DEVICE, _image.GetImages(), _image.GetImageCount(), _image.GetMetadata(), &_srv);


	
}

void Texture::Create(DXGI_FORMAT format, uint32 width, uint32 height,
	D3D11_BIND_FLAG flags,D3D11_USAGE usage, Vec4 clearColor)
{


	// DefaultTexture https://docs.microsoft.com/ko-kr/windows/win32/direct3d11/overviews-direct3d-11-resources-textures-create

	

	// DepthStencilTexture   https://docs.microsoft.com/ko-kr/windows/win32/direct3d11/d3d10-graphics-programming-guide-depth-stencil

	_desc.Width = width;
	_desc.Height = height;
	_desc.MipLevels = _desc.ArraySize = 1;
	_desc.Format = format;
	_desc.SampleDesc.Count = 1;
	_desc.SampleDesc.Quality = 0;
	_desc.Usage = usage;
	_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	_desc.MiscFlags = 0;
	_desc.CPUAccessFlags = 0;


	if (flags & D3D11_BIND_DEPTH_STENCIL)
	{
		_desc.BindFlags = flags;
	}
	else if(flags& D3D11_BIND_RENDER_TARGET)
	{
		_desc.BindFlags = flags | D3D11_BIND_SHADER_RESOURCE;
	}
	else if (flags & D3D11_BIND_UNORDERED_ACCESS)
	{
		_desc.BindFlags = flags | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	}


	HRESULT hr =  DEVICE->CreateTexture2D(&_desc, NULL, &_tex2D);


	CreateFromResource(_tex2D);
}

void Texture::CreateFromResource(ID3D11Texture2D* tex2D)
{
	_tex2D = tex2D;

	 tex2D->GetDesc(&_desc);

	// 주요 조합
	// - DSV 단독 (조합X)
	// - SRV
	// - RTV + SRV
	if (_desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		desc.Format = _desc.Format;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		DEVICE->CreateDepthStencilView(_tex2D, NULL, &_dsv);
	}
	else
	{
		if (_desc.BindFlags& D3D11_BIND_RENDER_TARGET)
		{
			// RTV
			
			D3D11_RENDER_TARGET_VIEW_DESC desc;
			desc.Format = _desc.Format;
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

			DEVICE->CreateRenderTargetView(_tex2D, nullptr, &_rtv);
		}

		if (_desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			// UAV
		

			D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
			desc.Format = _image.GetMetadata().format;
			desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

			DEVICE->CreateUnorderedAccessView(_tex2D,nullptr,&_uav);
		}

		if (_desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			// SRV
			D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};


			desc.Format = _desc.Format;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = 1;
			DEVICE->CreateShaderResourceView(_tex2D, nullptr, &_srv);
		}
		
	}
}