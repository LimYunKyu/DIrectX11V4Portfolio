#pragma once
class DXManager
{
public:
	DECLARE_SINGLE(DXManager)

public:
	void Init();
	void Update();
	void Render();
	void RenderBegine();
	void RenderEnd();

public:
	ID3D11DeviceContext* GetDeviceContext() { return _DeviceContext; }
	ID3D11Device* GetDevice() { return _Device; }
	ID3D11Texture2D* GeBacktBuffer() { return _backBuffer; }
private:

	void CreateDeviceAndSwapChain();
	void CreateViewPoart();
	

private:

	ID3D11Device* _Device;
	ID3D11DeviceContext* _DeviceContext;
	IDXGISwapChain* _SwapChain;
	ID3D11RenderTargetView* _RTV;
	D3D11_VIEWPORT _Viewport;

	ID3D11Texture2D* _backBuffer;
};

