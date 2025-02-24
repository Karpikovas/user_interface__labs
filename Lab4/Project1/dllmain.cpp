// dllmain.cpp: определяет точку входа для приложения DLL.
#include "stdafx.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
EXPORT void DrawLine(HDC hdc, int left, int top, int right, int bottom)
{
	MoveToEx(hdc, left, top, NULL);
	LineTo(hdc, right, bottom);
}
EXPORT void ClearScreen(HDC hdc, int left, int top, int right, int bottom)
{
	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
	HPEN hPen = (HPEN)SelectObject(hdc, CreatePen(PS_DOT, 2, RGB(255, 255, 255)));
	Rectangle(hdc, left, top, right, bottom);
}

