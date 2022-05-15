#pragma once

#include "ConstantBuffer.h"
class BufferManager
{
	DECLARE_SINGLE(BufferManager)
public:
	void CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize);
	void PushGraphicsSRV(SRV_REGISTER reg, ID3D11ShaderResourceView* srv);
	void PushComputeSRV(SRV_REGISTER reg, ID3D11ShaderResourceView* srv);
	void PushComputeUAV(UAV_REGISTER reg, ID3D11UnorderedAccessView* uav);

public:
	shared_ptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return _constantBuffers[static_cast<uint8>(type)]; }
private:
	vector<shared_ptr<ConstantBuffer>> _constantBuffers;
};

