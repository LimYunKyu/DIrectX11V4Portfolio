#include "pch.h"
#include "Material.h"
#include "Engine.h"
#include "BufferManager.h"
#include "DXManager.h"
#include "Shader.h"

Material::Material() : Object(OBJECT_TYPE::MATERIAL)
{

}

Material::~Material()
{

}

void Material::PushGraphicsData()
{

	// ���������� ����
	_shader->Update();

	// CBV ���ε�
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushGraphicsData(&_params, sizeof(_params));

	// SRV ���ε�
	for (size_t i = 0; i < _textures.size(); i++)
	{
		if (_textures[i] == nullptr)
			continue;

		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		
		GET_SINGLE(BufferManager)->PushGraphicsSRV(reg,_textures[i]->GetSRV());
	}


}

void Material::PushComputeData()
{
	// CBV ���ε�
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushComputeData(&_params, sizeof(_params));

	// SRV ���ε�
	for (size_t i = 0; i < _textures.size(); i++)
	{
		if (_textures[i] == nullptr)
			continue;

		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		GET_SINGLE(BufferManager)->PushComputeSRV(reg, _textures[i]->GetSRV());
		
	}

	// ���������� ����
	_shader->Update();
}

void Material::Dispatch(uint32 x, uint32 y, uint32 z)
{
	// CBV + SRV + SetPipelineState
	PushComputeData();

	// SetDescriptorHeaps + SetComputeRootDescriptorTable
	
	DEVICECONTEXT->Dispatch(x, y, z);
	

	
}

shared_ptr<Material> Material::Clone()
{
	shared_ptr<Material> material = make_shared<Material>();

	material->SetShader(_shader);
	material->_params = _params;
	material->_textures = _textures;

	return material;
}