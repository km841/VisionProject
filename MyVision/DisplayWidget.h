#pragma once
#include "pch.h"
#include "Widget.h"
class DisplayWidget :
    public Widget
{
public:
    DisplayWidget(const Vec2& _Size);
    virtual ~DisplayWidget();

public:
    virtual void Initialize() override;
    virtual void Update() override;

private:
    std::shared_ptr<class Texture> PTexture;
    
};

