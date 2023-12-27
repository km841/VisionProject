#pragma once
#include "Graphics.h"
#include "VisionGui.h"

class Application
{
	DECLARE_SINGLE(Application);

public:
	void Initialize(const WindowInfo& _WindowInfo);
	void Start();
	void Update();
	void Render();
	void Destroy();

public:
	std::shared_ptr<VisionGui> GetVisionGui() { return PVisionGui; }
	std::shared_ptr<Graphics> GetGraphics() { return PGraphics; }
	const WindowInfo& GetWindowInfo() { return MWindowInfo; }

private:
	void ResizeWindow(int _Width, int _Height);

private:
	WindowInfo MWindowInfo = {};

	std::shared_ptr<VisionGui> PVisionGui;
	std::shared_ptr<Graphics> PGraphics;
	
};

