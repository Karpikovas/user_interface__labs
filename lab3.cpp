// WindowsProject1.cpp: Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "WindowsProject1.h"

#define MAX_LOADSTRING 100
#define ID_EDITCHILD 100
#define WM_KEYDOWN                      0x0100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
TCHAR title1[] = _T("Window 1");
TCHAR title2[] = _T("Window 2");

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND window1 = nullptr;
HWND window2 = nullptr;
HWND edit1 = nullptr;
HWND edit2 = nullptr;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: разместите код здесь.

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

	window1 = CreateWindowW(szWindowClass, title1, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 500, 300, nullptr, nullptr, hInstance, nullptr);
	window2 = CreateWindowW(szWindowClass, title2, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 500, 300, nullptr, nullptr, hInstance, nullptr);
	edit1 = CreateWindowEx(
		0, L"EDIT",   // predefined class 
		NULL,         // no window title 
		WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
		0, 0, 500, 300,   // set size in WM_SIZE message 
		window1,         // parent window 
		(HMENU)ID_EDITCHILD,   // edit control ID 
		(HINSTANCE)GetWindowLong(window1, GWL_HINSTANCE),
		NULL);        // pointer not needed 
	edit2 = CreateWindowEx(
		0, L"EDIT",   // predefined class 
		NULL,         // no window title 
		WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
		0, 0, 500, 300,   // set size in WM_SIZE message 
		window2,         // parent window 
		(HMENU)ID_EDITCHILD,   // edit control ID 
		(HINSTANCE)GetWindowLong(window1, GWL_HINSTANCE),
		NULL);        // pointer not needed

	if (!window1 || !window2)
	{
		return FALSE;
	}

	ShowWindow(window1, nCmdShow);
	ShowWindow(window2, nCmdShow);
	UpdateWindow(window1);
	UpdateWindow(window2);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND: {
		TCHAR buff[1024];
		if (GetFocus() == edit1) {
			GetWindowText(edit1, buff, 1024);
			SetWindowText(edit2, buff);
		}
		else {
			GetWindowText(edit2, buff, 1024);
			SetWindowText(edit1, buff);
		}
	}
					 break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		EndPaint(hwnd, &ps);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
