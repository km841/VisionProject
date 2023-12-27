#include "Graphics.h"
#include "pch.h"
#include "Application.h"

void Graphics::Initialize()
{
	CreateDeviceAndContext();
	CreateDxgiFactory();
	CreateSwapChain();
	CreateSwapChainRTVAndSRV();
}

void Graphics::Render()
{
	float ClearColor[4] = { 0, 0, 0, 0 };
	PContext->ClearRenderTargetView(PRTV.Get(), ClearColor);
	PContext->OMSetRenderTargets(1, PRTV.GetAddressOf(), nullptr);
}

void Graphics::Present()
{
	PSwapChain->Present(0, 0);
}

void Graphics::CreateDeviceAndContext()
{	
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	HRESULT hResult = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE,
		0, creationFlags,
		featureLevels, ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION, &PDevice,
		0, &PContext);
	AssertEx(SUCCEEDED(hResult), L"Graphics::CreateDeviceAndContext() - Device 생성 실패");
}

void Graphics::CreateDxgiFactory()
{		
	HRESULT hResult = CreateDXGIFactory(IID_PPV_ARGS(&PDxgi));
	AssertEx(SUCCEEDED(hResult), L"Graphics::CreateDxgiFactory() - DxgiFactory 생성 실패");
}

void Graphics::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC sd = {};

	const WindowInfo& Info = APP->GetWindowInfo();

	sd.BufferDesc.Width = static_cast<UINT32>(Info.Width);
	sd.BufferDesc.Height = static_cast<UINT32>(Info.Height);
	sd.BufferDesc.RefreshRate.Numerator = 60; // 화면 갱신 비율
	sd.BufferDesc.RefreshRate.Denominator = 1; // 화면 갱신 비율
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 버퍼의 디스플레이 형식
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = Info.Hwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = PDxgi->CreateSwapChain(PDevice.Get(), &sd, &PSwapChain);
	AssertEx(SUCCEEDED(hr), L"SwapChain::CreateSwapChain() - SwapChain 생성 실패");
}

void Graphics::CreateSwapChainRTVAndSRV()
{
	ComPtr<ID3D11Texture2D> PBackBuffer;
	PSwapChain->GetBuffer(0, IID_PPV_ARGS(&PBackBuffer));

	PDevice->CreateRenderTargetView(PBackBuffer.Get(), nullptr, &PRTV);
	PDevice->CreateShaderResourceView(PBackBuffer.Get(), nullptr, &PSRV);
}
