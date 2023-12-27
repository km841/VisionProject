#pragma once
#include "pch.h"
class Texture;
class RenderManager
{
	DECLARE_SINGLE(RenderManager);

public:
	void Initialize();
	void Update();
	void Render();
	void Destroy();

public:
	void SetTexture(std::shared_ptr<Texture> _PTexture);
	void SetTexture(const std::wstring& _TextureName);
	std::shared_ptr<Texture> GetTexture() { return PTexture; }
	
private:
	void CreateShader(const std::wstring& _path, const std::string& _name, const std::string& _version, ComPtr<ID3DBlob>& _pBlob);
	void CreateVertexShader(const std::wstring& _path, const std::string& _name, const std::string& _version);
	void CreatePixelShader(const std::wstring& _path, const std::string& _name, const std::string& _version);

	void CreateSampler();
	void CreateInputLayout();

private:
	void CreateDefaultImage();

private:
	ComPtr<ID3D11VertexShader>      PVertexShader;
	ComPtr<ID3D11GeometryShader>    PGeometryShader;
	ComPtr<ID3D11PixelShader>       PPixelShader;
	ComPtr<ID3D11ComputeShader>     PComputeShader;

	ComPtr<ID3D11InputLayout>       PInputLayout;
	ComPtr<ID3D11SamplerState>      PSamplerState;

	ComPtr<ID3D11DepthStencilState> PDepthStencilState;
	ComPtr<ID3D11RasterizerState>   PRasterizerState;
	ComPtr<ID3D11BlendState>        PBlendState;

	ComPtr<ID3DBlob>                PVSBlob;
	ComPtr<ID3DBlob>                PPSBlob;
	ComPtr<ID3DBlob>                PGSBlob;
	ComPtr<ID3DBlob>                PCSBlob;
	ComPtr<ID3DBlob>                PErrBlob;

	std::shared_ptr<class Texture>  PTexture;
	std::wstring					MTexturePath = L"Empty";
};

