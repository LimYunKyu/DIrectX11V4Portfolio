#include "pch.h"
#include "StructuredBuffer.h"
#include "Engine.h"
#include "DXManager.h"

StructuredBuffer::StructuredBuffer()
{
}

StructuredBuffer::~StructuredBuffer()
{
}

void StructuredBuffer::Init(uint32 elementSize, uint32 elementCount, void* initialData)
{
	_elementSize = elementSize;
	_elementCount = elementCount;
	

	// Buffer
	{

		D3D11_BUFFER_DESC  sbDesc;
		sbDesc.ByteWidth = _elementSize;
		sbDesc.Usage = D3D11_USAGE_DYNAMIC;
		sbDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		sbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		sbDesc.MiscFlags = 0;
		sbDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sbData;
		sbData.pSysMem = nullptr;
		sbData.SysMemPitch = 0;
		sbData.SysMemSlicePitch = 0;

		if (initialData)
			CopyInitialData(_elementSize, initialData);

		DEVICE->CreateBuffer(&sbDesc, &sbData, &_buffer);

	}

	// SRV
	{
		

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		
		
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = _elementCount;
		srvDesc.Buffer.ElementWidth = _elementSize;
		

		DEVICE->CreateShaderResourceView(_buffer, &srvDesc, &_srv);
	}

	// UAV
	{

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};


		

		
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = _elementCount;
		

		DEVICE->CreateUnorderedAccessView(_buffer, &uavDesc, &_uav);
	}
}

void StructuredBuffer::PushGraphicsData(SRV_REGISTER reg)
{
	
	DEVICECONTEXT->VSSetShaderResources(static_cast<uint32>(reg), 1, &_srv);
	DEVICECONTEXT->PSSetShaderResources(static_cast<uint32>(reg), 1, &_srv);
}

void StructuredBuffer::PushComputeSRVData(SRV_REGISTER reg)
{
	DEVICECONTEXT->CSSetShaderResources(static_cast<uint32>(reg), 1, &_srv);
}

void StructuredBuffer::PushComputeUAVData(UAV_REGISTER reg)
{
	DEVICECONTEXT->CSSetUnorderedAccessViews(static_cast<uint32>(reg), 1, &_uav,0);
}

void StructuredBuffer::CopyInitialData(uint64 bufferSize, void* initialData)
{

	ID3D11Buffer* readBuffer = nullptr;


	D3D11_BUFFER_DESC  sbDesc;
	
	sbDesc.ByteWidth = _elementSize;
	sbDesc.Usage = D3D11_USAGE_DYNAMIC;
	//sbDesc.BindFlags = D3D11_BIND_FLAG;
	sbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sbDesc.MiscFlags = 0;
	sbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sbData;
	sbData.pSysMem = &_data;
	sbData.SysMemPitch = 0;
	sbData.SysMemSlicePitch = 0;


	DEVICE->CreateBuffer(&sbDesc, &sbData, &readBuffer);

	ZeroMemory(&_copymappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	DEVICECONTEXT->Map(readBuffer, 0, D3D11_MAP_WRITE_DISCARD,0, &_copymappedResource);


	memcpy(&_copymappedResource, initialData, bufferSize);
		
	DEVICECONTEXT->Unmap(readBuffer, 0);
	

	DEVICECONTEXT->CopyResource(_buffer, readBuffer);
	
}