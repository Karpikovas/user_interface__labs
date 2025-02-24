// Lab4.cpp: Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab4.h"

#define MAX_LOADSTRING 100
#define ID_BUTTON 3001

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];
TCHAR szChildClass[MAX_LOADSTRING] = _T("Graph");// имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
BOOL RegClass(WNDPROC, LPCTSTR, UINT);
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndChild(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

const int buttons_count = 4;
const int buttons_id_delta = 1000;
const int buttons_id_first = 100;

int checked_button_id = 0;
int checked_radio_button_id = 0;

HWND red, green, blue, line, rect, ell, cls;

bool click = false;
bool clear = false;
int x = 0, y = 0;

void(*ClearScreen)(HDC, int, int, int, int);
void(*DrawLine)(HDC, int, int, int, int);

COLORREF color = RGB(0, 0, 0);
int figure = -1;
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
    LoadStringW(hInstance, IDC_LAB4, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
	if (!RegClass(WndChild, szChildClass, COLOR_BTNFACE))
		return FALSE;

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB4));

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

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB4));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL RegClass(WNDPROC Proc, LPCTSTR szName, UINT brBackground)
{
	WNDCLASS wc;
	wc.style = wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.lpfnWndProc = Proc;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = (LPCTSTR)NULL;
	wc.lpszClassName = szName;
	return (RegisterClass(&wc) != 0);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

	DWORD window_style;
	int button_index;
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 500, nullptr, nullptr, hInstance, nullptr);
	HWND graph = CreateWindowEx(NULL, szChildClass, L"", WS_CHILD
 | WS_VISIBLE, 50, 0, 800, 500, hWnd, NULL, NULL, NULL);
 
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
//typedef void(__cdecl *ClearScreenType)(HDC, int, int, int, int);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool click = false;
	HINSTANCE hLib = NULL;
	hLib = ::LoadLibrary(L"Project1.dll");
    switch (message)
    {
	case WM_CREATE:
		{
			red = CreateWindowEx(0, L"BUTTON", L"Red", WS_GROUP | BS_PUSHLIKE | BS_AUTORADIOBUTTON | WS_VISIBLE | WS_CHILD, 0, 0, 50, 50, hWnd
				, 0, NULL, NULL);
			green = CreateWindowEx(0, L"BUTTON", L"Green", BS_PUSHLIKE | BS_AUTORADIOBUTTON | WS_VISIBLE | WS_CHILD, 0, 50, 50, 50, hWnd
				, 0, NULL, NULL);
			blue = CreateWindowEx(0, L"BUTTON", L"Blue", BS_PUSHLIKE | BS_AUTORADIOBUTTON | WS_VISIBLE | WS_CHILD, 0, 100, 50, 50, hWnd
				, 0, NULL, NULL);
			line = CreateWindowEx(0, L"BUTTON", L"Line", WS_GROUP | BS_PUSHLIKE | BS_AUTORADIOBUTTON | WS_VISIBLE | WS_CHILD, 0, 170, 50, 50, hWnd
				, 0, NULL, NULL);
			rect = CreateWindowEx(0, L"BUTTON", L"Rect", BS_PUSHLIKE | BS_AUTORADIOBUTTON | WS_VISIBLE | WS_CHILD, 0, 220, 50, 50, hWnd
				, 0, NULL, NULL);
			ell = CreateWindowEx(0, L"BUTTON", L"Ell", BS_PUSHLIKE | BS_AUTORADIOBUTTON | WS_VISIBLE | WS_CHILD, 0, 270, 50, 50, hWnd
				, 0, NULL, NULL);
			cls = CreateWindowEx(0, L"BUTTON", L"Cls", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 0, 340, 50, 50, hWnd
				, 0, NULL, NULL);


			if (hLib == NULL)
			{
				MessageBox(NULL, L"DLL Loading Failed.", L"Error", MB_OK | MB_ICONERROR);
				return FALSE;
			}
			(FARPROC &)ClearScreen = GetProcAddress((HMODULE)hLib, "ClearScreen");
			(FARPROC &)DrawLine = GetProcAddress((HMODULE)hLib, "DrawLine");
			if (!ClearScreen)
			{
				MessageBox(NULL, L"DLL Load2 Failed.", L"Error", MB_OK | MB_ICONERROR);
				return FALSE;
			}
		}
		break;
		
    case WM_COMMAND:
        {
			if (HWND(lParam) == red) {
				color = RGB(255, 0, 0);
			}
			else if (HWND(lParam) == green) {
				color = RGB(0, 255, 0);
			}
			else if (HWND(lParam) == blue) {
				color = RGB(0, 0, 255);
			}
			if (HWND(lParam) == line) {
				figure = 0;
			}
			else if (HWND(lParam) == rect) {
				figure = 1;
			} 
			else if (HWND(lParam) == ell) {
				figure = 2;
			} 
			if (HWND(lParam) == cls)
			{
				
				HDC dc = GetDC(hWnd);
				ClearScreen(dc, 51, 0, 800, 500);
				ReleaseDC(hWnd, dc);
			}
				
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
	FreeLibrary(hLib);
}

LRESULT CALLBACK WndChild(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		if (click) {
			int newX = LOWORD(lParam);
			int newY = HIWORD(lParam);
			HDC dc = GetDC(hWnd);
			RECT window;
			HBRUSH brush = CreateSolidBrush(color);
			HPEN hPen = (HPEN)SelectObject(dc, CreatePen(PS_DOT, 2, color));
			SelectObject(dc, brush);
			if (figure == 0)
			{
				DrawLine(dc, x, y, newX, newY);
			} 
			else if (figure == 1)
				Rectangle(dc, x, y, newX, newY);
			else if (figure == 2)
			{
				if (newX - x >= newY - y)
					Ellipse(dc, x, y, newX, y + newX - x);
				else
					Ellipse(dc, x, y, x + newY - y, newY);
			}
				
			DeleteObject(brush); //Очищаем память от созданной кисти

			ReleaseDC(hWnd, dc);
			click = !click;
		}
		else {
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			click = !click;
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


