#include "pch.h"
#include "Shader.h"
#include "Engine.h"
#include "DXManager.h"

Shader::Shader() : Object(OBJECT_TYPE::SHADER)
{

}

Shader::~Shader()
{
	
}

void Shader::CreateGraphicsShader(const wstring& path, ShaderInfo info, ShaderArg arg)
{
	_info = info;


   ////////////////////////////////////////////////
   //                                            //
   //                  Shader                    //
   //                                            //
   ////////////////////////////////////////////////




	CreateVertexShader(path, arg.vs, "vs_5_0");
	CreatePixelShader(path, arg.ps, "ps_5_0");

	if (arg.hs.empty() == false)
		CreateHullShader(path, arg.hs, "hs_5_0");

	if (arg.ds.empty() == false)
		CreateDomainShader(path, arg.ds, "ds_5_0");

	if (arg.gs.empty() == false)
		CreateGeometryShader(path, arg.gs, "gs_5_0");

	D3D11_INPUT_ELEMENT_DESC desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "W", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,  D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "W", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "W", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "W", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "WV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "WV", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "WV", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 96, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "WV", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 112, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "WVP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 128, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "WVP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 144, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "WVP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 160, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "WVP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 176, D3D11_INPUT_PER_INSTANCE_DATA, 1},
	};

	DEVICE->CreateInputLayout(desc, _countof(desc), _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), &_layout);
	_vsBlob->Release();
	

   ////////////////////////////////////////////////  https://docs.microsoft.com/ko-kr/windows/win32/direct3d11/d3d10-graphics-programming-guide-rasterizer-stage-getting-started
   //                                            //
   //                 Rasterizer                 //
   //                                            //
   ////////////////////////////////////////////////


	

	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthBias = false;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.ScissorEnable = true;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;
	

	switch (info.rasterizerType)
	{
	case RASTERIZER_TYPE::CULL_BACK:
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		break;
	case RASTERIZER_TYPE::CULL_FRONT:
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_FRONT;
		break;
	case RASTERIZER_TYPE::CULL_NONE:
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		break;
	case RASTERIZER_TYPE::WIREFRAME:
		rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		break;
	}



	DEVICE->CreateRasterizerState(&rasterizerDesc, &_rasterizerState);



  ////////////////////////////////////////////////  https://docs.microsoft.com/ko-kr/windows/win32/direct3d11/d3d10-graphics-programming-guide-depth-stencil
  //                                            //
  //                 DepthStencil               //
  //                                            //
  ////////////////////////////////////////////////


	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	

	switch (info.depthStencilType)
	{
	case DEPTH_STENCIL_TYPE::LESS:
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		break;
	case DEPTH_STENCIL_TYPE::LESS_EQUAL:
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		break;
	case DEPTH_STENCIL_TYPE::GREATER:
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthFunc = D3D11_COMPARISON_GREATER;
		break;
	case DEPTH_STENCIL_TYPE::GREATER_EQUAL:
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		break;
	case DEPTH_STENCIL_TYPE::NO_DEPTH_TEST:
		dsDesc.DepthEnable = FALSE;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;;
		break;
	case DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE:
		dsDesc.DepthEnable = FALSE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		break;
	case DEPTH_STENCIL_TYPE::LESS_NO_WRITE:
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		break;
	}

	DEVICE->CreateDepthStencilState(&dsDesc, &_depthStencilState);


 ////////////////////////////////////////////////  https://docs.microsoft.com/ko-kr/windows/win32/direct3d11/d3d10-graphics-programming-guide-blend-state
 //                                            //
 //                 Blend		               //
 //                                            //
 ////////////////////////////////////////////////

	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(D3D11_BLEND_DESC));
	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	auto br = BlendDesc.RenderTarget[0];

	switch (info.blendType)
	{
	case BLEND_TYPE::DEFAULT:
		br.BlendEnable = FALSE;		
		br.SrcBlend = D3D11_BLEND_ONE;
		br.DestBlend = D3D11_BLEND_ZERO;
		break;
	case BLEND_TYPE::ALPHA_BLEND:
		br.BlendEnable = TRUE;	
		br.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		br.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		break;
	case BLEND_TYPE::ONE_TO_ONE_BLEND:
		br.BlendEnable = TRUE;
		br.SrcBlend = D3D11_BLEND_ONE;
		br.DestBlend = D3D11_BLEND_ONE;
		break;
	}

	DEVICE->CreateBlendState(&BlendDesc, &_blendState);
	

	//Sampler
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	DEVICE->CreateSamplerState(&sampDesc, &_samplerState);
	



}

void Shader::CreateComputeShader(const wstring& path, const string& name, const string& version)
{
	_info.shaderType = SHADER_TYPE::COMPUTE;

	CreateShader(path, name, version, &_csBlob);
	DEVICE->CreateComputeShader(_csBlob->GetBufferPointer(), _csBlob->GetBufferSize(), NULL, &_computeShader);

	_csBlob->Release();
}

void Shader::Update()
{
	if (GetShaderType() == SHADER_TYPE::COMPUTE)
	{

		DEVICECONTEXT->CSSetShader(_computeShader, NULL, 0);
		
	}
	else
	{
		DEVICECONTEXT->VSSetShader(_vertexShader, NULL, 0);
		DEVICECONTEXT->PSSetShader(_pixelShader, NULL, 0);
		DEVICECONTEXT->PSSetSamplers(0, 1, &_samplerState);
		DEVICECONTEXT->IASetPrimitiveTopology(_info.topology);
		DEVICECONTEXT->RSSetState(_rasterizerState);
		DEVICECONTEXT->OMSetDepthStencilState(_depthStencilState, 0);
		DEVICECONTEXT->IASetInputLayout(_layout);
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		UINT sampleMask = 0xffffffff;

		DEVICECONTEXT->OMSetBlendState(_blendState, blendFactor, sampleMask);
	}
		
	
	
	

	

}

void Shader::CreateShader(const wstring& path, const string& name, const string& version,ID3DBlob** blob)
{

	

	uint32 compileFlag = 0;
#ifdef _DEBUG
	compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	if (FAILED(::D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, name.c_str(), version.c_str(), compileFlag, 0, blob, &_errBlob)))
	{
		::MessageBoxA(nullptr, "Shader Create Failed !", nullptr, MB_OK);
	}

	

	
}

void Shader::CreateVertexShader(const wstring& path, const string& name, const string& version)
{
	CreateShader(path, name, version, &_vsBlob);

	DEVICE->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), NULL, &_vertexShader);

	

}

void Shader::CreateHullShader(const wstring& path, const string& name, const string& version)
{
	CreateShader(path, name, version, &_hsBlob);
	DEVICE->CreateHullShader(_hsBlob->GetBufferPointer(), _hsBlob->GetBufferSize(), NULL, &_hullShader);

	_hsBlob->Release();
}

void Shader::CreateDomainShader(const wstring& path, const string& name, const string& version)
{
	CreateShader(path, name, version, &_dsBlob);
	DEVICE->CreateDomainShader(_dsBlob->GetBufferPointer(), _dsBlob->GetBufferSize(), NULL, &_domainShader);

	_dsBlob->Release();
}

void Shader::CreateGeometryShader(const wstring& path, const string& name, const string& version)
{
	CreateShader(path, name, version, &_gsBlob);
	DEVICE->CreateGeometryShader(_gsBlob->GetBufferPointer(), _gsBlob->GetBufferSize(), NULL, &_geometryShader);

	_gsBlob->Release();
}

void Shader::CreatePixelShader(const wstring& path, const string& name, const string& version)
{
	CreateShader(path, name, version, &_psBlob);
	DEVICE->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), NULL, &_pixelShader);

	_psBlob->Release();
}

D3D11_PRIMITIVE_TOPOLOGY Shader::GetTopologyType(D3D_PRIMITIVE_TOPOLOGY topology)
{
	/*switch (topology)
	{
	case D3D_PRIMITIVE_TOPOLOGY_POINTLIST:
		return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;;
	case D3D_PRIMITIVE_TOPOLOGY_LINELIST:
	case D3D_PRIMITIVE_TOPOLOGY_LINESTRIP:
	case D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ:
	case D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ:
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ:
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	case D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
	default:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
	}*/

	return D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
}
