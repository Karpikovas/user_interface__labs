// Lab5.cpp: Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab5.h"
#include <commctrl.h>
#pragma comment (lib, "comctl32")
#define ID_EDITCHILD 100
#define ID_STATIC1 199
#define IDC_LISTVIEW 101
#define IDC_BTNOK 102
#define IDC_BTNNEXT 103
#define ID_TABLE 104
#define ID_COMBOBOX 1001

#define MAX_LOADSTRING 100
enum states
{
	state_1,
	state_2,
	state_3,
	state_4,
	state_5,
	state_6,
	state_7,
	state_8,
	state_9,
	state_10,
	state_11,
	state_12,
	state_13
};

enum states current_state = state_1;

HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];  

BOOL RegClass(WNDPROC Proc, LPCTSTR szName, UINT brBackground);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HWND CreateListView(HWND hWndParent, UINT uId);

HWND hEditControlFormula;
HWND hListFields;

HWND hSelect;
HWND hBtnOK;
HWND hBtnNext;

HWND hwndResTable;
HWND hOperationMode;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	hInst = hInstance;

	if (!RegClass(WndProc, L"Main", COLOR_GRADIENTACTIVECAPTION)) { return FALSE; };

	HWND hWnd = CreateWindowW(L"Main", L"Main", WS_OVERLAPPEDWINDOW|WS_VISIBLE,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);


	if (!hWnd)
	{
		MessageBox(NULL, L"Window Creation Failed.", L"Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


BOOL RegClass(WNDPROC Proc, LPCTSTR szName, UINT brBackground)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = Proc;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(brBackground + 1);
	wc.lpszMenuName = (LPCTSTR)NULL;
	wc.lpszClassName = szName;
	return (RegisterClass(&wc) != 0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR text[1024] = TEXT("");

    switch (message)
    {
	case WM_CREATE:
	{

		hOperationMode = CreateWindowEx(WS_EX_STATICEDGE, L"COMBOBOX", L"MyCombo2",
			CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE| WS_VSCROLL,
			30, 30, 150, 100, hWnd, (HMENU)ID_COMBOBOX, hInst, NULL);
		ComboBox_AddString(hOperationMode, TEXT("Режим таблицы"));
		ComboBox_AddString(hOperationMode, TEXT("Режим запроса"));

		hEditControlFormula = CreateWindowEx(0, L"EDIT",   // predefined class 
			L"Введите таблицу",         // no window title 
			WS_CHILD | WS_VISIBLE |ES_LEFT | WS_BORDER,
			30, 70, 150, 20,   // set size in WM_SIZE message 
			hWnd,         // parent window 
			(HMENU)ID_EDITCHILD,   // edit control ID 
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);
		EnableWindow(hEditControlFormula, FALSE);
		hListFields = CreateWindowEx(0, L"listbox", NULL,
			WS_CHILD | WS_VISIBLE | LBS_STANDARD,
			210, 30,
			150, 200,
			hWnd,
			(HMENU)IDC_LISTVIEW,
			hInst, NULL);
		EnableWindow(hListFields, FALSE);
		hSelect = CreateWindowEx(0, L"STATIC", NULL,
			WS_CHILD | WS_VISIBLE,
			490, 30,
			200, 165,
			hWnd, nullptr, hInst, NULL);
		EnableWindow(hSelect, FALSE);

		hBtnOK = CreateWindowEx(0, L"button", L"OK",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			400, 30,
			50, 30,
			hWnd, (HMENU)IDC_BTNOK, hInst, NULL);
		EnableWindow(hBtnOK, FALSE);

		hBtnNext = CreateWindowEx(0, L"button", L"Далее",
			WS_CHILD |WS_VISIBLE| BS_PUSHBUTTON,
			400, 80,
			50, 30,
			hWnd, (HMENU)IDC_BTNNEXT, hInst, NULL);
		EnableWindow(hBtnNext, FALSE);

		hwndResTable = CreateListView(hWnd, ID_TABLE);

	}
		break;
    case WM_COMMAND:
        {
		int wmId = LOWORD(wParam);
		switch (current_state) {
			case state_1:
				switch (wmId)
				{
					case ID_COMBOBOX:
						int id = SendMessage(hOperationMode, CB_GETCURSEL, 0, 0);
						if (id != -1) {
							current_state = state_2;
							EnableWindow(hEditControlFormula, true);
							EnableWindow(hBtnOK, true);
							SetFocus(hBtnOK);
							EnableWindow(hBtnNext, FALSE);

							SendMessage(hEditControlFormula, EM_SETSEL, 0, -1);
							SendMessage(hEditControlFormula, WM_CLEAR, 0, 0);
						}
						else
							break;
						if (id == 1) {
							EnableWindow(hBtnNext, TRUE);
						}

						break;
				}
				break;
			case state_2:
				break;
			case state_3:
				break;
			case state_4:
				break;
			case state_5:
				break;
			case state_6:
				break;
			case state_7:
				break;
			case state_8:
				break;
			case state_9:
				break;
			case state_10:
				break;
			case state_11:
				break;
			case state_12:
				break;
			case state_13:
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
		}/*
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
			case IDC_BTNOK:
				ShowWindow(hBtnNext, SW_SHOW);
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }*/
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


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
HWND CreateListView(HWND hWndParent, UINT uId)
{
	INITCOMMONCONTROLSEX icex;
	//icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	RECT rcl;
	GetClientRect(hWndParent, &rcl);

	HWND hWndLV = CreateWindowEx(NULL, WC_LISTVIEW, NULL,
		WS_CHILD|WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_REPORT | LVS_EDITLABELS | LVS_EX_FULLROWSELECT,
		0, 250,
		600, 350,
		hWndParent, (HMENU)uId, hInst, NULL);


	// Чтобы определялись строка (item) и столбец (subitem) обязательно устанавливаем
	// расширенный стиль LVS_EX_FULLROWSELECT.
	ListView_SetExtendedListViewStyleEx(hWndLV, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	LVCOLUMN structColumn;
	memset(&structColumn, 0, sizeof(structColumn));
	structColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	structColumn.fmt = LVCFMT_LEFT;
	structColumn.cx = 100; //ширина столбца

	ListView_InsertColumn(hwndResTable, 0, &structColumn);
	ListView_InsertColumn(hwndResTable, 1, &structColumn);
	ListView_InsertColumn(hwndResTable, 2, &structColumn);

	return (hWndLV);
}