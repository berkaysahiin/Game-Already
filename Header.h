#pragma once

#define GAME_NAME "GAME_ALREADY"

#define GAME_RES_WIDTH 384

#define GAME_RES_HEIGHT 216

#define GAME_BPP 32

#define GAME_DRAWING_AREA_MEMORY_SIZE (GAME_RES_WIDTH * GAME_RES_HEIGHT * (GAME_BPP/8))

typedef struct GAMEBITMAP
{
	BITMAPINFO bitMapInfo;

	void* memory;

} GAMEBITMAP;

LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam);

DWORD CreateMainGameWindow(void);

BOOL GameIsAlreadyRunning(void);

void ProcessPlayerInput(void);

void RenderFrameGraphics(void);