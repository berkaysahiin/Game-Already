#include <stdio.h>

#pragma warning(push, 3)

#include <windows.h>

#pragma warning(pop)

#include <stdint.h>

#include "Header.h"

HWND gGameWindow;

BOOL gGameIsRunning;

GAMEBITMAP gBackBuffer;

MONITORINFO gMonitorInfo = { sizeof(MONITORINFO) } ;


int WINAPI WinMain (HINSTANCE Instance, HINSTANCE PreviousInstance, LPSTR CommandLine, int CmdShow)
{
	UNREFERENCED_PARAMETER(PreviousInstance);

	UNREFERENCED_PARAMETER(CommandLine);

    UNREFERENCED_PARAMETER(CmdShow);

    UNREFERENCED_PARAMETER(Instance);

    if (GameIsAlreadyRunning() == TRUE)
    {
        MessageBoxA(NULL, "Another instance of this program is already running", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;

    }

    if (CreateMainGameWindow() != ERROR_SUCCESS)
    {
        goto Exit;
    }

    gBackBuffer.bitMapInfo.bmiHeader.biSize = sizeof(gBackBuffer.bitMapInfo.bmiHeader);

    gBackBuffer.bitMapInfo.bmiHeader.biWidth = GAME_RES_WIDTH;

    gBackBuffer.bitMapInfo.bmiHeader.biHeight = GAME_RES_HEIGHT;

    gBackBuffer.bitMapInfo.bmiHeader.biBitCount = GAME_BPP;

    gBackBuffer.bitMapInfo.bmiHeader.biCompression = BI_RGB;

    gBackBuffer.bitMapInfo.bmiHeader.biPlanes = 1;

    gBackBuffer.memory = VirtualAlloc(NULL, GAME_DRAWING_AREA_MEMORY_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if (gBackBuffer.memory == NULL)
    {
        MessageBoxA(NULL, "Failed to allocate memory for drawing surface", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

    MSG Message = { 0 };

    gGameIsRunning = TRUE;

    while (gGameIsRunning == TRUE)
    {
        while (PeekMessageA(&Message, gGameWindow, 0 , 0, PM_REMOVE))
        {
            DispatchMessage(&Message);
        }

        ProcessPlayerInput();

        RenderFrameGraphics();

        Sleep(1);

        
    }

 Exit:
	return (0);
}

LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam)
{
    LRESULT Result = 0;

    switch (Message)
    {
        case WM_CLOSE:
        {
            gGameIsRunning = FALSE;

            PostQuitMessage(0);

            break;
        }

        default:
        {
            Result = DefWindowProcA(WindowHandle, Message, WParam, LParam);
        }
    }
    return(Result);
}

DWORD CreateMainGameWindow(void)
{
    DWORD Result = ERROR_SUCCESS;

    WNDCLASSEXA WindowClass = { 0 };

    WindowClass.cbSize = sizeof(WNDCLASSEX);

    WindowClass.style = 0;

    WindowClass.lpfnWndProc = MainWindowProc;

    WindowClass.cbClsExtra = 0;

    WindowClass.cbWndExtra = 0;

    WindowClass.hInstance = GetModuleHandle(NULL);

    WindowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);

    WindowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);

    WindowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);

    WindowClass.hbrBackground = CreateSolidBrush(RGB(255, 0, 255));

    WindowClass.lpszMenuName = NULL;

    WindowClass.lpszClassName = "GAME_ALREADY_WÝNDOWCLASS";


    if (RegisterClassEx(&WindowClass) == 0)
    {
        Result = GetLastError();

        MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

    gGameWindow = CreateWindowExA(WS_EX_CLIENTEDGE, WindowClass.lpszClassName, "Windows", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (gGameWindow == NULL)
    {
        Result = GetLastError();

        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

    if (GetMonitorInfoA(MonitorFromWindow(gGameWindow, MONITOR_DEFAULTTOPRIMARY), &gMonitorInfo) == 0)
    {
        Result = ERROR_MONITOR_NO_DESCRIPTOR;

        MessageBox(NULL, "Could not acces to your monitor", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

    int MonitorWidth = gMonitorInfo.rcMonitor.right - gMonitorInfo.rcMonitor.left;

    int MonitorHeight = gMonitorInfo.rcMonitor.top - gMonitorInfo.rcMonitor.bottom;

 Exit:
    return(Result);
}

BOOL GameIsAlreadyRunning(void)
{
    HANDLE Mutex = NULL;

    Mutex = CreateMutexA(NULL, FALSE,  "GAME_ALREADY_MUTEX");

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

void ProcessPlayerInput(void)
{
    short EscapeKeyIsDown = GetAsyncKeyState(VK_ESCAPE);

    if (EscapeKeyIsDown)
    {
        SendMessageA(gGameWindow, WM_CLOSE, 0, 0);
    }
}

void RenderFrameGraphics(void)
{
    HDC DeviceContext = GetDC(gGameWindow);

    StretchDIBits(DeviceContext, 0, 0, 100, 100, 0, 0, 100, 100, gBackBuffer.memory, &gBackBuffer.bitMapInfo, DIB_RGB_COLORS, SRCCOPY);

    ReleaseDC(gGameWindow, DeviceContext);
}
