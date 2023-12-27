#pragma once

#include <windows.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <functional>
#include <mutex>
#include <queue>
#include <type_traits>
#include <optional>
#include <bitset>
#include <numeric>
#include <filesystem>
#include <algorithm>
#include <typeinfo>
#include <typeindex>
#include <random>
#include <assert.h>

#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "d3d11.h"
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <dwrite.h>
#include <d2d1.h>
#include <dshow.h>

#include "imgui_internal.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "ImGuiFileDialog.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex.lib")
#endif

#include "EnumDefine.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace fs = std::filesystem;
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

struct WindowInfo
{
	HWND Hwnd;
	int Width;
	int Height;
};

using Vec2 = ImVec2;

static void AssertEx(bool _expression, const std::wstring& _message)
{
	if (_expression)
		return;

	MessageBoxW(NULL, _message.c_str(), L"Assert", MB_OK);
	assert(_expression);
}

#define DECLARE_SINGLE(type)  \
private:					  \
	type();				  \
	~type();				  \
							  \
public:						  \
	static type* GetInstance()\
	{						  \
		static type instance; \
		return &instance;	  \
	}						  \


#define GET_SINGLE(type) type::GetInstance()
#define APP GET_SINGLE(Application)
#define DEVICE APP->GetGraphics()->GetDevice()
#define CONTEXT APP->GetGraphics()->GetContext()
#define RENDER GET_SINGLE(RenderManager)

std::wstring s2ws(const std::string& s);
std::string ws2s(const std::wstring& ws);