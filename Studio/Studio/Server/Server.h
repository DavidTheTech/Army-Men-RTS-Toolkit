#pragma once
#include <d3d9.h>
#include <Windows.h>

class Server
{
public:
    void init();

private:
    static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    bool CreateDeviceD3D(HWND hWnd);
    void CleanupDeviceD3D();
    void CreateRenderTarget();
    void CleanupRenderTarget();
    void RenderFrame();

    LPDIRECT3D9 g_pD3D;
    LPDIRECT3DDEVICE9 g_pd3dDevice;
    D3DPRESENT_PARAMETERS g_d3dpp;
    HWND g_hwnd;

    bool showExitPopup;
    bool shouldClose;
    bool isDragging;
    POINT dragOffset;

    static Server* s_instance;
};