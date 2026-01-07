#include "Server.h"
#include "Imgui\imgui.h"
#include "Imgui\imgui_impl_win32.h"
#include "Imgui\imgui_impl_dx9.h"

#pragma comment(lib, "d3d9.lib")

Server* Server::s_instance = nullptr;

void Server::init()
{
    s_instance = this;

    g_pD3D = nullptr;
    g_pd3dDevice = nullptr;
    g_hwnd = nullptr;
    showExitPopup = false;
    shouldClose = false;
    isDragging = false;
    dragOffset = { 0, 0 };

    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"AMRTS SERV CTRL", nullptr };
    RegisterClassExW(&wc);

    g_hwnd = CreateWindowW(wc.lpszClassName, L"Army Men RTS Server Controls", WS_POPUP, 100, 100, 420, 200, nullptr, nullptr, wc.hInstance, nullptr);

    if (!CreateDeviceD3D(g_hwnd))
    {
        CleanupDeviceD3D();
        UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return;
    }

    ShowWindow(g_hwnd, SW_SHOWDEFAULT);
    UpdateWindow(g_hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsClassic();

    ImFontConfig config;
    config.MergeMode = false;

    ImGui_ImplWin32_Init(g_hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    bool done = false;

    while (!done)
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
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

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        
        ImGui::SetNextWindowSize(ImVec2(420, 200), ImGuiCond_Always);
        ImGui::Begin("Army Men RTS Server Controls", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        if (ImGui::Button("Hello World"))
        {

        }

        ImGui::End();

        if (showExitPopup)
        {
            ImGui::OpenPopup("Confirm Exit");
            showExitPopup = false;
        }

        if (ImGui::BeginPopupModal("Confirm Exit", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Are you sure you want to exit?");
            ImGui::Separator();

            if (ImGui::Button("Yes", ImVec2(80, 0)))
            {
                shouldClose = true;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("No", ImVec2(80, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::Render();

        if (shouldClose)
        {
            PostQuitMessage(0);
        }

        RenderFrame();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    DestroyWindow(g_hwnd);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

bool Server::CreateDeviceD3D(HWND hWnd)
{
    g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!g_pD3D)
    {
        return false;
    }

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
    {
        return false;
    }

    return true;
}

void Server::CleanupDeviceD3D()
{
    if (g_pd3dDevice)
    {
        g_pd3dDevice->Release();
        g_pd3dDevice = nullptr;
    }

    if (g_pD3D)
    {
        g_pD3D->Release();
        g_pD3D = nullptr;
    }
}

void Server::CreateRenderTarget()
{

}

void Server::CleanupRenderTarget()
{

}

void Server::RenderFrame()
{
    static DWORD lastTime = 0;
    DWORD currentTime = GetTickCount();
    DWORD targetFrameTime = 1000 / 60;

    if (currentTime - lastTime < targetFrameTime)
    {
        Sleep(targetFrameTime - (currentTime - lastTime));
    }
    lastTime = GetTickCount();

    g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

    if (g_pd3dDevice->BeginScene() >= 0)
    {
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        g_pd3dDevice->EndScene();
    }

    HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

    if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
    {
        ImGui_ImplDX9_InvalidateDeviceObjects();
        g_pd3dDevice->Reset(&g_d3dpp);
        ImGui_ImplDX9_CreateDeviceObjects();
    }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

LRESULT WINAPI Server::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        return true;
    }

    Server* server = s_instance;
    if (!server)
    {
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }

    switch (msg)
    {
        case WM_LBUTTONDOWN:
        {
            POINT cursor;
            GetCursorPos(&cursor);
            ScreenToClient(server->g_hwnd, &cursor);

            if (cursor.y < 19)
            {
                server->isDragging = true;
                server->dragOffset.x = cursor.x;
                server->dragOffset.y = cursor.y;
                SetCapture(server->g_hwnd);
            }
            break;
        }

        case WM_LBUTTONUP:
        {
            server->isDragging = false;
            ReleaseCapture();
            break;
        }

        case WM_MOUSEMOVE:
        {
            if (server->isDragging)
            {
                POINT cursor;
                GetCursorPos(&cursor);

                RECT rect;
                GetWindowRect(server->g_hwnd, &rect);

                SetWindowPos(server->g_hwnd, NULL, cursor.x - server->dragOffset.x, cursor.y - server->dragOffset.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            }
            break;
        }

        case WM_SIZE:
        {
            if (server->g_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
            {
                server->g_d3dpp.BackBufferWidth = LOWORD(lParam);
                server->g_d3dpp.BackBufferHeight = HIWORD(lParam);
                ImGui_ImplDX9_InvalidateDeviceObjects();
                server->g_pd3dDevice->Reset(&server->g_d3dpp);
                ImGui_ImplDX9_CreateDeviceObjects();
            }
            return 0;
        }

        case WM_CLOSE:
        {
            server->showExitPopup = true;
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

    return DefWindowProcW(hWnd, msg, wParam, lParam);
}