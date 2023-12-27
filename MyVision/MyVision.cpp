#include "pch.h"
#include "MyVision.h"
#include "Application.h"
#include "VisionGui.h"

HINSTANCE hInst;
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

const wchar_t* GWindowName = L"Vision";
WindowInfo GWindowInfo;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    WNDCLASSEX wcex{ 0 };
    wcex.cbSize = sizeof(wcex);
    wcex.lpszClassName = GWindowName;
    wcex.lpfnWndProc = WndProc;
    wcex.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
    wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
    wcex.hInstance = hInstance;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;

    ATOM atom = RegisterClassEx(&wcex);

    GWindowInfo.Width = 1920;
    GWindowInfo.Height = 1080;

    HWND hwnd = CreateWindowW(GWindowName, GWindowName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    AssertEx(hwnd, L"wWinMain() - Create WindowHandle Failed");
    GWindowInfo.Hwnd = hwnd;

    ShowWindow(hwnd, SW_SHOW);
    MSG msg{ 0 };

    APP->Initialize(GWindowInfo);
    APP->Start();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(1));

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        APP->Update();
        APP->Render();
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (APP->GetVisionGui()->DispatchWndMessage(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        APP->Destroy();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

