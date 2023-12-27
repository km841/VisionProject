#pragma once
#include "pch.h"
#include "Widget.h"
class DirectoryWidget :
    public Widget
{
public:
    DirectoryWidget(const Vec2& _Size);
    virtual ~DirectoryWidget();

public:
    virtual void Initialize() override;
    virtual void Update() override;

private:
    std::string MDefaultPath = "..\\Resources\\Texture\\";
    std::vector<std::string> VDirectoryImagePath;
    std::shared_ptr<class Texture> PIconTexture;

    int MSelectedImageIndex;
};

