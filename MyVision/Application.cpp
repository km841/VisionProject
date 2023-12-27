#include "pch.h"
#include "Application.h"
#include "VisionGui.h"
#include "Graphics.h"
#include "RenderManager.h"

Application::Application()
{

}

Application::~Application()
{

}

void Application::Initialize(const WindowInfo& _WindowInfo)
{
	MWindowInfo = _WindowInfo;
	ResizeWindow(MWindowInfo.Width, MWindowInfo.Height);

	PVisionGui = std::make_shared<VisionGui>();
	PGraphics = std::make_shared<Graphics>();

	PGraphics->Initialize();
	PVisionGui->Initialize(MWindowInfo.Hwnd, PGraphics->GetDevice(), PGraphics->GetContext());

	GET_SINGLE(RenderManager)->Initialize();
}

void Application::Start()
{
}

void Application::Update()
{
	GET_SINGLE(RenderManager)->Update();
	PVisionGui->Update();
}

void Application::Render()
{
	PGraphics->Render();

	GET_SINGLE(RenderManager)->Render();
	PVisionGui->Render();

	PGraphics->Present();
}

void Application::Destroy()
{
	PVisionGui->Destroy();
	PVisionGui.reset();
}

void Application::ResizeWindow(int _Width, int _Height)
{
	MWindowInfo.Width = _Width;
	MWindowInfo.Height = _Height;

	RECT Rect = { 0, 0, _Width, _Height };
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(MWindowInfo.Hwnd, 0, 0, 0, _Width, _Height, 0);
}
