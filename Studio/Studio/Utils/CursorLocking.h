#include <tchar.h>

RECT windowRect;
HWND targetWindow = NULL;

BOOL IsWindowOnTop(HWND hWnd)
{
    return (GetForegroundWindow() == hWnd);
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
    DWORD processId;
    GetWindowThreadProcessId(hWnd, &processId);

    if (processId == GetCurrentProcessId())
    {
        TCHAR windowTitle[256];
        GetWindowText(hWnd, windowTitle, sizeof(windowTitle) / sizeof(TCHAR));
        while (true)
        {
            if (_tcsstr(windowTitle, _T("Army Men RTS Studio")))
            {
                targetWindow = hWnd;
                return FALSE;
            }
        }
    }

    return TRUE;
}

void GetWindowDimensions(HWND hWnd)
{
    GetClientRect(hWnd, &windowRect);
    MapWindowPoints(hWnd, NULL, reinterpret_cast<POINT*>(&windowRect), 2);
}

void LockCursorInWindow()
{
    int shrink = 2;
    POINT cursorPos;
    GetCursorPos(&cursorPos);

    int left = windowRect.left + shrink;
    int right = windowRect.right - shrink;
    int top = windowRect.top + shrink;
    int bottom = windowRect.bottom - shrink;

    if (!PtInRect(&windowRect, cursorPos))
    {
        if (cursorPos.x < left)   cursorPos.x = left;
        if (cursorPos.x > right)  cursorPos.x = right;
        if (cursorPos.y < top)    cursorPos.y = top;
        if (cursorPos.y > bottom) cursorPos.y = bottom;

        SetCursorPos(cursorPos.x, cursorPos.y);
    }
}