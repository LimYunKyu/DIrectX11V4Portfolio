#pragma once

class StructuredBuffer
{
public:
	StructuredBuffer();
	~StructuredBuffer();

	void Init(uint32 elementSize, uint32 elementCount, void* initialData = nullptr);

	void PushGraphicsData(SRV_REGISTER reg);
	void PushComputeSRVData(SRV_REGISTER reg);
	void PushComputeUAVData(UAV_REGISTER reg);
	

	ID3D11ShaderResourceView*	 GetSRV() { return _srv; }
	ID3D11UnorderedAccessView*	 GetUAV() { return _uav; }

	
	
	ID3D11Buffer* GetBuffer() { return _buffer; }

	uint32	GetElementSize() { return _elementSize; }
	uint32	GetElementCount() { return _elementCount; }
	UINT	GetBufferSize() { return _elementSize * _elementCount; }

private:
	void CopyInitialData(uint64 bufferSize, void* initialData);

private:
	ID3D11Buffer*				_buffer;
	ID3D11ShaderResourceView*	_srv;
	ID3D11UnorderedAccessView*	_uav;
	D3D11_MAPPED_SUBRESOURCE	_mappedResource;
	D3D11_MAPPED_SUBRESOURCE	_copymappedResource;
	uint32						_elementSize = 0;
	uint32						_elementCount = 0;
	
	BYTE* _data;


};

