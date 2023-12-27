#pragma once
#include "pch.h"

class VisionGui
{
public:
	void Initialize(
		HWND _Hwnd, 
		ComPtr<ID3D11Device> _PDevice, 
		ComPtr<ID3D11DeviceContext> _PContext);

	void Update();
	void Render();
	void Destroy();

public:
	void AddWidget(class Widget* _Widget);

public:
	LRESULT DispatchWndMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	void InitImGui();
	void InitWidgets();

private:
	HWND MHwnd;
	ComPtr<ID3D11Device> PDevice;
	ComPtr<ID3D11DeviceContext> PContext;

	std::vector<class Widget*> VWidgets;
};

