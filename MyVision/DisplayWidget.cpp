#include "DisplayWidget.h"
#include "Texture.h"
#include "RenderManager.h"

DisplayWidget::DisplayWidget(const Vec2& _Size)
	: Widget(_Size)
{
}

DisplayWidget::~DisplayWidget()
{
}

void DisplayWidget::Initialize()
{
}

void DisplayWidget::Update()
{
	PTexture = GET_SINGLE(RenderManager)->GetTexture();
	if (ImGui::Begin("Display", nullptr))
	{
		ImGui::Image(PTexture->GetSRV().Get(), ImGui::GetWindowSize());
	}
	ImGui::End();
}
