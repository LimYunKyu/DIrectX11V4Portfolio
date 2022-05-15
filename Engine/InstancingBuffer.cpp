#include "pch.h"
#include "InstancingBuffer.h"
#include "Engine.h"
#include "DXManager.h"

InstancingBuffer::InstancingBuffer()
{

}

InstancingBuffer::~InstancingBuffer()
{
	DEVICECONTEXT->Unmap(_buffer, 0);
}

void InstancingBuffer::Init(uint32 maxCount)
{
	_maxCount = maxCount;

	D3D11_BUFFER_DESC  cbDesc;
	cbDesc.ByteWidth = sizeof(InstancingParams) * maxCount;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA cbData;
	cbData.pSysMem = &_data;
	cbData.SysMemPitch = 0;
	cbData.SysMemSlicePitch = 0;


	DEVICE->CreateBuffer(&cbDesc, &cbData, &_buffer);

	ZeroMemory(&_mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	DEVICECONTEXT->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mappedResource);
	
}

void InstancingBuffer::Clear()
{
	_data.clear();
}

void InstancingBuffer::AddData(InstancingParams& params)
{
	_data.push_back(params);
}

void InstancingBuffer::PushData()
{
	const uint32 dataCount = GetCount();
	if (dataCount > _maxCount)
		Init(dataCount);

	const uint32 bufferSize = dataCount * sizeof(InstancingParams);

	memcpy(_mappedResource.pData, &_data[0], bufferSize);

}