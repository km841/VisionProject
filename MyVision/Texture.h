#pragma once
#include "Object.h"
class Texture
    : public Object
{
public:
    Texture(OBJECT_TYPE _EObjectType);
    virtual ~Texture();

    virtual void Load(const std::wstring& _Path) override;

public:
    void Create(UINT32 _EType, DXGI_FORMAT _EFormat, int _Width, int _Height);
    void CreateFromMatrix(const cv::Mat& _Image);
    void CreateFromTexture(UINT _Type, DXGI_FORMAT _EFormat, ComPtr<ID3D11Texture2D> _PTex2D);

public:
    const std::wstring& GetPath() { return MPath; }
    bool IsProcessed() { return BProcessed; }
    void SetProcessed() { BProcessed = true; }

public:
    ComPtr<ID3D11Texture2D>           GetTex2D() const { return PTex2D; }
    ComPtr<ID3D11RenderTargetView>    GetRTV()   const { return PRTV; }
    ComPtr<ID3D11ShaderResourceView>  GetSRV()   const { return PSRV; }
    ComPtr<ID3D11DepthStencilView>    GetDSV()   const { return PDSV; }
    ComPtr<ID3D11UnorderedAccessView> GetUAV()   const { return PUAV; }

protected:
    UINT32                              ETextureType;
    std::wstring                        MPath;

    ScratchImage                        MScratchImage;
    ComPtr<ID3D11Texture2D>             PTex2D;

    ComPtr<ID3D11RenderTargetView>      PRTV;
    ComPtr<ID3D11ShaderResourceView>    PSRV;
    ComPtr<ID3D11DepthStencilView>      PDSV;
    ComPtr<ID3D11UnorderedAccessView>   PUAV;

    bool                                BProcessed = false;
};

