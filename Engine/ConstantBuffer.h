#pragma once

enum class CONSTANT_BUFFER_TYPE : uint8
{
	GLOBAL,
	TRANSFORM,
	MATERIAL,
	END
};

enum
{
	CONSTANT_BUFFER_COUNT = static_cast<uint8>(CONSTANT_BUFFER_TYPE::END)
};


class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

public:
	void Init(CBV_REGISTER reg, uint32 size);
	void PushGraphicsData(void* buffer, uint32 size);
	void PushComputeData(void* buffer, uint32 size);

private:

	CBV_REGISTER				_reg = {};
	BYTE*						_data;
	uint32						_size;

	ID3D11Buffer*				_buffer;
	ID3D11ShaderResourceView*	_srv;
	ID3D11UnorderedAccessView*	_uav;
	D3D11_MAPPED_SUBRESOURCE	_mappedResource;
};

