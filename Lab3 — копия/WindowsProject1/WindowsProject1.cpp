// WindowsProject1.cpp: Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "WindowsProject1.h"

#define ID_EDITCHILD 100

#include <windows.h>

#include <commctrl.h>




TCHAR ClassName[] = _T("MainWindowClass");

TCHAR ChildClassName[] = _T("ChildWindowClass");

HWND hWndClient;

HINSTANCE hInst;


LRESULT CALLBACK ChildProc(HWND    hWnd,

	UINT    Msg,

	WPARAM  wParam,

	LPARAM  lParam)

{
	switch (Msg)
	{
	case WM_CREATE: {
		HWND hEdit;

		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
			WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE |
			ES_WANTRETURN,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			hWnd, (HMENU)IDC_CHILD_EDIT, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		SendMessage(hEdit, WM_SETFONT,
			(WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	}
	}




	return DefMDIChildProc(hWnd, Msg, wParam, lParam);

}





LRESULT CALLBACK WndProc(HWND    hWnd,

	UINT    Msg,

	WPARAM  wParam,

	LPARAM  lParam)

{

	switch (Msg)

	{

	case WM_CREATE:

	{

		CLIENTCREATESTRUCT ccs;

		ccs.hWindowMenu = GetSubMenu(GetMenu(hWnd), 0);

		ccs.idFirstChild = ID_MDI_FIRSTCHILD;





		hWndClient = CreateWindowEx(

			WS_EX_CLIENTEDGE,

			L"MDICLIENT",

			NULL,

			WS_CHILD | WS_VISIBLE | WS_VSCROLL |
			ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,

			CW_USEDEFAULT,

			CW_USEDEFAULT,

			CW_USEDEFAULT,

			CW_USEDEFAULT,

			hWnd,

			(HMENU)ID_MDI_CLIENT,

			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),

			(LPVOID)&ccs);



		if (!hWndClient)

		{

			MessageBox(hWnd, L"Failed To Create The Client Window", L"Error", MB_OK);

		}



		ShowWindow(hWndClient, SW_SHOW);

	}

	break;



	case WM_COMMAND:

	{

		switch (LOWORD(wParam))

		{

		case IDM_FILE_EXIT:

			PostMessage(hWnd, WM_CLOSE, 0, 0);

			break;



		case IDM_FILE_NEW:

		{

			HWND hChild;

			CREATESTRUCT cs;

			ZeroMemory(&cs, sizeof(CREATESTRUCT));



			hChild = CreateWindowEx(

				WS_EX_MDICHILD,

				L"ChildWindowClass",

				L"Child Window",

				WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE |
				ES_WANTRETURN,

				CW_USEDEFAULT,

				CW_USEDEFAULT,

				CW_USEDEFAULT,

				CW_USEDEFAULT,

				hWndClient,

				(HMENU)2000,

				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),

				&cs);
			





			if (!hChild)

				MessageBox(hWnd, L"Failed To Create The Child Window", L"Error", MB_OK);

		}

		break;



		default:

		{

			if (LOWORD(wParam) >= ID_MDI_FIRSTCHILD)

				DefFrameProc(hWnd, hWndClient, Msg, wParam, lParam);

			else

			{

				HWND hChild;

				hChild = (HWND)SendMessage(hWndClient, WM_MDIGETACTIVE, 0, 0);

				if (hChild)

					SendMessage(hChild, WM_COMMAND, wParam, lParam);

			}

		}

		}

		return 0;

	}

	break;



	case WM_CLOSE:

		DestroyWindow(hWnd);

		break;



	case WM_DESTROY:

		PostQuitMessage(0);

		break;



	default:

		return DefFrameProc(hWnd, hWndClient, Msg, wParam, lParam);

	}



	return 0;

}



INT WINAPI WinMain(HINSTANCE  hInstance,

	HINSTANCE  hPrevInstance,

	LPSTR      lpCmdLine,

	INT        nCmdShow)

{

	//InitCommonControls();

	hInst = hInstance;

	WNDCLASSEX    wc;



	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;

	wc.lpfnWndProc = (WNDPROC)WndProc;

	wc.cbClsExtra = 0;

	wc.cbWndExtra = 0;

	wc.hInstance = hInstance;

	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));

	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));

	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);

	wc.lpszClassName = L"MainWindowClass";



	if (!RegisterClassEx(&wc))

	{

		MessageBox(NULL, L"Failed To Register The Window Class.", L"Error", MB_OK | MB_ICONERROR);

		return 0;

	}



	wc.lpfnWndProc = (WNDPROC)ChildProc;

	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	wc.lpszMenuName = NULL;

	wc.lpszClassName = L"ChildWindowClass";



	if (!RegisterClassEx(&wc))

	{

		MessageBox(NULL, L"Failed To Register The Child Window Class", L"Error", MB_OK | MB_ICONERROR);

		return 0;

	}



	HWND    hWnd;



	hWnd = CreateWindowEx(

		WS_EX_CLIENTEDGE,

		L"MainWindowClass",

		L"MDI",

		WS_OVERLAPPEDWINDOW,

		CW_USEDEFAULT,

		CW_USEDEFAULT,

		600,

		400,

		NULL,

		NULL,

		hInstance,

		NULL);



	if (!hWnd)

	{

		MessageBox(NULL,L"Window Creation Failed.", L"Error", MB_OK | MB_ICONERROR);

		return 0;

	}



	ShowWindow(hWnd, SW_SHOW);

	UpdateWindow(hWnd);



	MSG    Msg;



	while (GetMessage(&Msg, NULL, 0, 0))

	{

		if (!TranslateMDISysAccel(hWndClient, &Msg))

		{

			TranslateMessage(&Msg);

			DispatchMessage(&Msg);

		}

	}



	return Msg.wParam;

}