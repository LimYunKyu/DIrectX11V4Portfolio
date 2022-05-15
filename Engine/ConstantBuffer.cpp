#include "pch.h"
#include "ConstantBuffer.h"
#include "DXManager.h"

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
	//DEVICECONTEXT->Unmap(_buffer,0);

}

void ConstantBuffer::Init(CBV_REGISTER reg, uint32 size)
{
	_reg = reg;
	_size = (size + 255) & ~255;

 ////////////////////////////////////////////////  https://docs.microsoft.com/en-us/windows/win32/direct3d11/how-to--use-dynamic-resources
 //                                            //
 //        Create Dynamic Constant Buffer      //
 //                                            //
 ////////////////////////////////////////////////


	D3D11_BUFFER_DESC  cbDesc;
	cbDesc.ByteWidth = _size;
	//cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	/*D3D11_SUBRESOURCE_DATA cbData;
	cbData.pSysMem = &_data;
	cbData.SysMemPitch = 0;
	cbData.SysMemSlicePitch = 0;*/

	
	DEVICE->CreateBuffer(&cbDesc, nullptr, &_buffer);

	/*ZeroMemory(&_mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	DEVICECONTEXT->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mappedResource);*/



}

void ConstantBuffer::PushGraphicsData(void* buffer, uint32 size)
{

	//memcpy(_mappedResource.pData, buffer, size);


	DEVICECONTEXT->UpdateSubresource(_buffer, 0, NULL, buffer, 0, 0);
	uint32 reg = static_cast<uint32>(_reg);
	DEVICECONTEXT->VSSetConstantBuffers(reg, 1, &_buffer);
	DEVICECONTEXT->PSSetConstantBuffers(reg, 1, &_buffer);


}

void ConstantBuffer::PushComputeData(void* buffer, uint32 size)
{
	//memcpy(_mappedResource.pData, buffer, size);
	DEVICECONTEXT->UpdateSubresource(_buffer, 0, NULL, buffer, 0, 0);
	uint32 reg = static_cast<uint32>(_reg);
	DEVICECONTEXT->CSSetConstantBuffers(reg, 1, &_buffer);
	
}


