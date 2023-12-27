#pragma once
#include "pch.h"
class Graphics
{
public:
	Graphics() = default;
	~Graphics() = default;

public:
	void Initialize();
	void Render();
	void Present();

public:
	ComPtr<ID3D11DeviceContext> GetContext() { return PContext; }
	ComPtr<ID3D11Device> GetDevice() { return PDevice; }
	ComPtr<IDXGISwapChain> GetSwapChain() { return PSwapChain; }
	ComPtr<ID3D11ShaderResourceView> GetBackBufferSRV() { return PSRV; }
	ComPtr<ID3D11RenderTargetView> GetBackBufferRTV() { return PRTV; }

private:
	void CreateDeviceAndContext();
	void CreateDxgiFactory();
	void CreateSwapChain();
	void CreateSwapChainRTVAndSRV();

private:
	ComPtr<ID3D11DeviceContext> PContext;
	ComPtr<ID3D11Device> PDevice;
	ComPtr<IDXGIFactory> PDxgi;
	ComPtr<IDXGISwapChain> PSwapChain;
	ComPtr<ID3D11RenderTargetView> PRTV;
	ComPtr<ID3D11ShaderResourceView> PSRV;
};

