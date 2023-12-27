#include "pch.h"
#include "Texture.h"
#include "Application.h"


Texture::Texture(OBJECT_TYPE _EObjectType)
	: Object(_EObjectType)
{
}

Texture::~Texture()
{
}

void Texture::Load(const std::wstring& _Path)
{
	MPath = _Path;
	std::wstring Ext = fs::path(_Path).extension();

	HRESULT hr = 0;

	if (Ext == L".dds" || Ext == L".DDS")
		hr = LoadFromDDSFile(_Path.c_str(), DDS_FLAGS_NONE, nullptr, MScratchImage);
	else if (Ext == L".tga" || Ext == L".TGA")
		hr = LoadFromTGAFile(_Path.c_str(), nullptr, MScratchImage);
	else // png, jpg, jpeg, bmp
		hr = LoadFromWICFile(_Path.c_str(), WIC_FLAGS_NONE, nullptr, MScratchImage);

	AssertEx(SUCCEEDED(hr), L"Texture::Load() - 텍스쳐 로드 실패, 경로 문제일 가능성 높음");

	hr = CreateShaderResourceView(
		DEVICE.Get(),
		MScratchImage.GetImages(),
		MScratchImage.GetImageCount(),
		MScratchImage.GetMetadata(),
		PSRV.GetAddressOf());

	AssertEx(SUCCEEDED(hr), L"Texture::Load() - 텍스쳐 SRV 생성 실패");

	PSRV->GetResource(reinterpret_cast<ID3D11Resource**>(PTex2D.GetAddressOf()));
	MName = _Path;
}

void Texture::Create(UINT32 _EType, DXGI_FORMAT _EFormat, int _Width, int _Height)
{
	D3D11_TEXTURE2D_DESC td = { 0 };

	td.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	td.CPUAccessFlags = 0;
	td.Width = _Width;
	td.Height = _Height;
	td.ArraySize = 1;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.MipLevels = 1;
	td.MiscFlags = 0;
	td.BindFlags = _EType;

	if (_EType & D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL)
		td.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	else
		td.Format = _EFormat;

	HRESULT hr = DEVICE->CreateTexture2D(&td, nullptr, PTex2D.GetAddressOf());
	AssertEx(SUCCEEDED(hr), L"Texture::Create() - Texture2D 생성 실패");

	CreateFromTexture(_EType, _EFormat, PTex2D);
}

void Texture::CreateFromMatrix(const cv::Mat& _Image)
{
	int width = _Image.cols;
	int height = _Image.rows;

	cv::Mat BgraMat;
	cv::cvtColor(_Image, BgraMat, cv::COLOR_BGR2BGRA);

	D3D11_TEXTURE2D_DESC Desc;
	Desc.Width = BgraMat.cols;
	Desc.Height = BgraMat.rows;
	Desc.MipLevels = 1;
	Desc.ArraySize = 1;
	Desc.Format = DXGI_FORMAT_B8G8R8X8_UNORM; // 형식을 BGR로 설정
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	Desc.CPUAccessFlags = 0;
	Desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = BgraMat.data;
	InitData.SysMemPitch = static_cast<UINT>(BgraMat.step);
	InitData.SysMemSlicePitch = 0;

	HRESULT hr = DEVICE->CreateTexture2D(&Desc, &InitData, PTex2D.GetAddressOf());
	AssertEx(SUCCEEDED(hr), L"Texture::CreateFromMatrix() - Texture2D 생성 실패");

	hr = DEVICE->CreateShaderResourceView(PTex2D.Get(), nullptr, &PSRV);
	AssertEx(SUCCEEDED(hr), L"Texture::CreateFromMatrix() - 텍스쳐 SRV 생성 실패");
}

void Texture::CreateFromTexture(UINT _Type, DXGI_FORMAT _EFormat, ComPtr<ID3D11Texture2D> _PTex2D)
{
	ETextureType = _Type;
	PTex2D = _PTex2D;

	D3D11_TEXTURE2D_DESC td = {};
	PTex2D->GetDesc(&td);

	if (ETextureType & D3D11_BIND_RENDER_TARGET)
	{
		D3D11_RENDER_TARGET_VIEW_DESC rd;
		rd.Format = DXGI_FORMAT_UNKNOWN;
		rd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		HRESULT hr = DEVICE->CreateRenderTargetView(PTex2D.Get(), nullptr, PRTV.GetAddressOf());

		AssertEx(SUCCEEDED(hr), L"Texture::CreateFromTexture() - 텍스쳐 RTV 생성 실패");
	}

	if (ETextureType & D3D11_BIND_SHADER_RESOURCE)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC sd = {};
		sd.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		sd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		sd.Texture2D.MipLevels = 1;

		HRESULT hr = DEVICE->CreateShaderResourceView(PTex2D.Get(), &sd, PSRV.GetAddressOf());
		AssertEx(SUCCEEDED(hr), L"Texture::CreateFromTexture() - 텍스쳐 SRV 생성 실패");
	}

	if (ETextureType & D3D11_BIND_DEPTH_STENCIL)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC dsd = {};
		dsd.Format = DXGI_FORMAT_UNKNOWN;
		dsd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		HRESULT hr = DEVICE->CreateDepthStencilView(PTex2D.Get(), nullptr, PDSV.GetAddressOf());

		AssertEx(SUCCEEDED(hr), L"Texture::CreateFromTexture() - 텍스쳐 DSV 생성 실패");
	}

	if (ETextureType & D3D11_BIND_UNORDERED_ACCESS)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC ud = {};
		ud.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		ud.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
		HRESULT hr = DEVICE->CreateUnorderedAccessView(PTex2D.Get(), &ud, PUAV.GetAddressOf());
		AssertEx(SUCCEEDED(hr), L"Texture::CreateFromTexture() - 텍스쳐 UAV 생성 실패");
	}
}
