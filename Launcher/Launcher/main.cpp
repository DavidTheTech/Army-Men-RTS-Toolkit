// main.cpp
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <shellapi.h>
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_win32.h"
#include "IMGUI/imgui_impl_dx11.h"
#include "GameLauncher.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "shell32.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        return true;
    }

    switch (msg)
    {
        //creates the "fake" title bar that allows window dragging
        case WM_NCHITTEST:
        {
            POINT pt = { LOWORD(lParam), HIWORD(lParam) };

            ScreenToClient(hWnd, &pt);
            if (pt.y >= 0 && pt.y <= 32)
            {
                RECT rect;
                GetClientRect(hWnd, &rect);
                
                //minus the ? _ X buttons
                if (pt.x < rect.right - 95)
                {
                    return HTCAPTION;
                }
            }
            break;
        }
        case WM_SIZE:
        {
            if (wParam != SIZE_MINIMIZED)
            {
            }
            return 0;
        }
        case WM_SYSCOMMAND:
        {
            if ((wParam & 0xfff0) == SC_KEYMENU)
            {
                return 0;
            }
            break;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    WNDCLASSEX wc =
    {
        sizeof(WNDCLASSEX), CS_CLASSDC, WndProc,
        0L, 0L, GetModuleHandle(NULL),
        NULL, NULL, NULL,
        NULL, L"ArmyMenRTSLauncher", NULL
    };
    RegisterClassEx(&wc);

    int winWidth = 350;
    int winHeight = 650;

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, L"Army Men RTS Launcher", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, winWidth, winHeight, NULL, NULL, wc.hInstance, NULL);

    if (!hwnd)
    {
        return 1;
    }

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(hwnd, NULL, (screenWidth - winWidth) / 2, (screenHeight - winHeight) / 2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;

    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    IDXGISwapChain* swapchain = nullptr;
    ID3D11RenderTargetView* rtv = nullptr;

    D3D_FEATURE_LEVEL featureLevel;
    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &swapchain, &device, &featureLevel, &context);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    //no imgui.ini
    io.IniFilename = NULL;

    ImFontConfig config;
    config.OversampleH = 2;
    config.OversampleV = 2;
    config.SizePixels = 18.0f;
    ImFont* font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/seguisym.ttf", 18.0f, &config);

    if (!font)
    {
        font = io.Fonts->AddFontDefault();
    }

    GameLauncher app(device, hwnd);
    app.SetupTheme();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(device, context);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    bool done = false;
    app.LoadJoke();

    while (!done)
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
            {
                done = true;
            }
        }

        if (done)
        {
            break;
        }

        RECT rect;
        GetClientRect(hwnd, &rect);
        if (rtv)
        {
            rtv->Release();
            rtv = nullptr;
        }
        swapchain->ResizeBuffers(0, rect.right - rect.left, rect.bottom - rect.top, DXGI_FORMAT_UNKNOWN, 0);

        ID3D11Texture2D* backbuffer = nullptr;
        swapchain->GetBuffer(0, IID_PPV_ARGS(&backbuffer));
        device->CreateRenderTargetView(backbuffer, nullptr, &rtv);
        backbuffer->Release();

        context->OMSetRenderTargets(1, &rtv, nullptr);

        float clearColor[4] = { 0.09f, 0.09f, 0.15f, 1.00f };
        context->ClearRenderTargetView(rtv, clearColor);

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        app.Render();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        swapchain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    if (rtv)
    {
        rtv->Release();
    }

    if (swapchain)
    {
        swapchain->Release();
    }

    if (context)
    {
        context->Release();
    }

    if (device)
    {
        device->Release();
    }

    DestroyWindow(hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}