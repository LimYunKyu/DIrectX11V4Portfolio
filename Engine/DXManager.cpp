#include "pch.h"
#include "Engine.h"
#include "DXManager.h"



void DXManager::Init()
{
    CreateDeviceAndSwapChain();
}

void DXManager::Update()
{
}

void DXManager::Render()
{
    
    float ClearColor[4] = { 0.5f, 0.5f, 1.0f, 1.0f };
    _DeviceContext->ClearRenderTargetView(_RTV, ClearColor);

   // //Present the backbuffer to the screen
    _SwapChain->Present(0, 0);
}

void DXManager::RenderBegine()
{
}

void DXManager::RenderEnd()
{
    _SwapChain->Present(0, 0);
}

void DXManager::CreateDeviceAndSwapChain()
{
    //Describe our Buffer
    DXGI_MODE_DESC bufferDesc;

    ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

    bufferDesc.Width = GEngine->GetWinfo().width;
    bufferDesc.Height = GEngine->GetWinfo().height;
    bufferDesc.RefreshRate.Numerator = 60;
    bufferDesc.RefreshRate.Denominator = 1;
    bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    //Describe our SwapChain
    DXGI_SWAP_CHAIN_DESC swapChainDesc;

    ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

    swapChainDesc.BufferDesc = bufferDesc;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = GEngine->GetWinfo().hwnd;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


    //Create our SwapChain
    D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
        D3D11_SDK_VERSION, &swapChainDesc, &_SwapChain, &_Device, NULL, &_DeviceContext);

    //Create our BackBuffer
    _SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&_backBuffer);

   // DEVICE->CreateRenderTargetView(_backBuffer, nullptr, &_RTV);

}

void DXManager::CreateViewPoart()
{
    ZeroMemory(&_Viewport, sizeof(D3D11_VIEWPORT));
    _Viewport.TopLeftX = 0;
    _Viewport.TopLeftY = 0;
    _Viewport.Width = GEngine->GetWinfo().width;
    _Viewport.Height = GEngine->GetWinfo().height;

    _DeviceContext->RSSetViewports(1, &_Viewport);
}


