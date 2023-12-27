#include "RenderManager.h"
#include "Application.h"
#include "Texture.h"

RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
}

void RenderManager::Initialize()
{
	CreateVertexShader(L"..\\Resources\\Shader\\DefaultShader.fx", "VS_Main", "vs_5_0");
	CreatePixelShader(L"..\\Resources\\Shader\\DefaultShader.fx", "PS_Main", "ps_5_0");

	CreateSampler();
	CreateInputLayout();

	CreateDefaultImage();
}

void RenderManager::Update()
{
	CONTEXT->VSSetSamplers(0, 1, PSamplerState.GetAddressOf());
	CONTEXT->PSSetSamplers(0, 1, PSamplerState.GetAddressOf());

	CONTEXT->VSSetShader(PVertexShader.Get(), nullptr, 0);
	CONTEXT->PSSetShader(PPixelShader.Get(), nullptr, 0);

	CONTEXT->IASetInputLayout(PInputLayout.Get());
}

void RenderManager::Render()
{
}

void RenderManager::Destroy()
{
}

void RenderManager::SetTexture(std::shared_ptr<Texture> _PTexture)
{
	PTexture = _PTexture;
	MTexturePath = _PTexture->GetPath();
}

void RenderManager::SetTexture(const std::wstring& _TexturePath)
{
	if (_TexturePath == MTexturePath)
	{
		return;
	}

	std::shared_ptr<Texture> NewTexture = std::make_shared<Texture>(OBJECT_TYPE::TEXTURE);
	NewTexture->Load(_TexturePath);

	MTexturePath = _TexturePath;
	PTexture = NewTexture;
}

void RenderManager::CreateShader(const std::wstring& _Path, const std::string& _Name, const std::string& _Version, ComPtr<ID3DBlob>& _PBlob)
{
	int CompileFlag = 0;
#ifdef _DEBUG
	CompileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = D3DCompileFromFile(_Path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_Name.c_str(), _Version.c_str(), CompileFlag, 0, &_PBlob, &PErrBlob);

	if (FAILED(hr))
	{
		const char* errorString = NULL;
		if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
			errorString = "Could not compile shader; file not found";
		else if (PErrBlob) {
			errorString = (const char*)PErrBlob->GetBufferPointer();
			PErrBlob->Release();
		}
		AssertEx(NULL, L"RenderManager::CreateShader() - Shader Compiler Error");
	}
}

void RenderManager::CreateVertexShader(const std::wstring& _Path, const std::string& _Name, const std::string& _Version)
{
	CreateShader(_Path, _Name, _Version, PVSBlob);
	HRESULT hr = DEVICE->CreateVertexShader(PVSBlob->GetBufferPointer(), PVSBlob->GetBufferSize(), nullptr, &PVertexShader);
	AssertEx(SUCCEEDED(hr), L"RenderManager::CreateVertexShader() - CreateVertexShader Failed");
}

void RenderManager::CreatePixelShader(const std::wstring& _Path, const std::string& _Name, const std::string& _Version)
{
	CreateShader(_Path, _Name, _Version, PPSBlob);
	HRESULT hr = DEVICE->CreatePixelShader(PPSBlob->GetBufferPointer(), PPSBlob->GetBufferSize(), nullptr, &PPixelShader);
	AssertEx(SUCCEEDED(hr), L"RenderManager::CreatePixelShader() - CreatePixelShader Failed");
}

void RenderManager::CreateSampler()
{
	D3D11_SAMPLER_DESC SamplerDesc = {};
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;

	SamplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	HRESULT hr = DEVICE->CreateSamplerState(&SamplerDesc, PSamplerState.GetAddressOf());
	AssertEx(SUCCEEDED(hr), L"RenderManager::CreateSampler() - Create Linear Sampler Failed");
}

void RenderManager::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	HRESULT hr = DEVICE->CreateInputLayout(InputLayoutDesc, ARRAYSIZE(InputLayoutDesc), PVSBlob->GetBufferPointer(), PVSBlob->GetBufferSize(), &PInputLayout);
	AssertEx(SUCCEEDED(hr), L"RenderManager::CreateInputLayout() - CreateInputLayout Failed");
}

void RenderManager::CreateDefaultImage()
{
	PTexture = std::make_shared<Texture>(OBJECT_TYPE::TEXTURE);
	PTexture->Load(L"..\\Resources\\Texture\\Default.png");
}
