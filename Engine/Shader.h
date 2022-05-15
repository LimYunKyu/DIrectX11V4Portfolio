#pragma once
#include "Object.h"

enum class SHADER_TYPE : uint8
{
	DEFERRED,
	FORWARD,
	LIGHTING,
	PARTICLE,
	COMPUTE,
	SHADOW,
};

enum class RASTERIZER_TYPE : uint8
{
	CULL_NONE,
	CULL_FRONT,
	CULL_BACK,
	WIREFRAME,
};

enum class DEPTH_STENCIL_TYPE : uint8
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	NO_DEPTH_TEST, // ±Ì¿Ã ≈◊Ω∫∆Æ(X) + ±Ì¿Ã ±‚∑œ(O)
	NO_DEPTH_TEST_NO_WRITE, // ±Ì¿Ã ≈◊Ω∫∆Æ(X) + ±Ì¿Ã ±‚∑œ(X)
	LESS_NO_WRITE, // ±Ì¿Ã ≈◊Ω∫∆Æ(O) + ±Ì¿Ã ±‚∑œ(X)
};

enum class BLEND_TYPE : uint8
{
	DEFAULT,
	ALPHA_BLEND,
	ONE_TO_ONE_BLEND,
	END,
};

struct ShaderInfo
{
	SHADER_TYPE shaderType = SHADER_TYPE::FORWARD;
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
	BLEND_TYPE blendType = BLEND_TYPE::DEFAULT;
	D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

struct ShaderArg
{
	const string vs = "VS_Main";
	const string hs;
	const string ds;
	const string gs;
	const string ps = "PS_Main";
};

class Shader : public Object
{
public:
	Shader();
	virtual ~Shader();

	void CreateGraphicsShader(const wstring& path, ShaderInfo info = ShaderInfo(), ShaderArg arg = ShaderArg());
	void CreateComputeShader(const wstring& path, const string& name, const string& version);

	void Update();

	SHADER_TYPE GetShaderType() { return _info.shaderType; }

	static D3D11_PRIMITIVE_TOPOLOGY GetTopologyType(D3D_PRIMITIVE_TOPOLOGY topology);

private:
	void CreateShader(const wstring& path, const string& name, const string& version, ID3DBlob** blob);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreateHullShader(const wstring& path, const string& name, const string& version);
	void CreateDomainShader(const wstring& path, const string& name, const string& version);
	void CreateGeometryShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);

private:
	ShaderInfo _info;
	ID3D11InputLayout* _layout;
	
	// GraphicsShader
	ID3DBlob*					    _vsBlob;
	ID3DBlob*						_hsBlob;
	ID3DBlob*						_dsBlob;
	ID3DBlob*						_gsBlob;
	ID3DBlob*						_psBlob;
	ID3DBlob*						_errBlob;
	

	// ComputeShader
	ID3DBlob*						_csBlob;

	ID3D11VertexShader*				_vertexShader;
	ID3D11DomainShader*				_domainShader;
	ID3D11HullShader*				_hullShader;
	ID3D11GeometryShader*			_geometryShader;
	ID3D11PixelShader*				_pixelShader;
	
	ID3D11ComputeShader*			_computeShader;

	ID3D11RasterizerState*			_rasterizerState;
	ID3D11DepthStencilState*		_depthStencilState;
	ID3D11BlendState*				_blendState;
	ID3D11SamplerState*				_samplerState;
	
};

