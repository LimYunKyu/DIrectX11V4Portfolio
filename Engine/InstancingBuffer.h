#pragma once

struct InstancingParams
{
	Matrix matWorld;
	Matrix matWV;
	Matrix matWVP;
};

class InstancingBuffer
{
public:
	InstancingBuffer();
	~InstancingBuffer();

	void Init(uint32 maxCount = 10);

	void Clear();
	void AddData(InstancingParams& params);
	void PushData();

public:
	uint32						GetCount() { return static_cast<uint32>(_data.size()); }
	ID3D11Buffer*				GetBuffer() { return _buffer; }
	ID3D11Buffer*				GetVertexBuffer() { return _vertexbuffer; }

	void	SetID(uint64 instanceId) { _instanceId = instanceId; }
	uint64	GetID() { return _instanceId; }

private:
	uint64						_instanceId = 0;
	ID3D11Buffer*				_buffer;
	ID3D11Buffer*				_vertexbuffer;
	D3D11_MAPPED_SUBRESOURCE	_mappedResource;

	uint32						_maxCount = 0;
	vector<InstancingParams>	_data;
};