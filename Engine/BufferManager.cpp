#include "pch.h"
#include "BufferManager.h"
#include "DXManager.h"


void BufferManager::CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize)
{
	uint8 typeInt = static_cast<uint8>(reg);
	assert(_constantBuffers.size() == typeInt);

	shared_ptr<ConstantBuffer> buffer = make_shared<ConstantBuffer>();
	buffer->Init(reg, bufferSize);
	_constantBuffers.push_back(buffer);


}

void BufferManager::PushGraphicsSRV(SRV_REGISTER reg, ID3D11ShaderResourceView* srv)
{
	uint32 _reg = static_cast<uint32>(reg);

	DEVICECONTEXT->VSSetShaderResources(_reg, 1, &srv);
	DEVICECONTEXT->PSSetShaderResources(_reg, 1, &srv);
}

void BufferManager::PushComputeSRV(SRV_REGISTER reg, ID3D11ShaderResourceView* srv)
{

	uint32 _reg = static_cast<uint32>(reg);

	DEVICECONTEXT->CSSetShaderResources(_reg, 1, &srv);
	
}

void BufferManager::PushComputeUAV(UAV_REGISTER reg, ID3D11UnorderedAccessView* uav)
{
	uint32 _reg = static_cast<uint32>(reg);
	DEVICECONTEXT->CSSetUnorderedAccessViews(_reg, 1, &uav,0);
}
