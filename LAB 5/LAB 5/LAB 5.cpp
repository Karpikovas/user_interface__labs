#include "stdafx.h"
#include "LAB 5.h"
#include <commctrl.h>
#pragma comment (lib, "comctl32")
#include <Windows.h>
#include <windowsx.h>
#include "lib/sqlite3.h"
#pragma comment (lib, "lib/sqlite3.lib")
#include <string>
#include <cstdio>
#include "atlstr.h"

#define ID_COMBOBOX 1001
#define ID_TABLEMENU 1000
#define ID_SELECTMENU 1003
#define ID_EXIT 1004
#define ID_EDITCHILD 100
#define ID_STATIC1 199
#define IDC_LISTVIEW 101
#define IDC_LISTVIEW1 110
#define IDC_BTNOK 102
#define IDC_BTNNEXT 103
#define IDC_BTNUP 108
#define IDC_BTNDOWN 109
#define IDC_BTNUP 108
#define IDC_BTNEXIT 112
#define ID_TABLE 104
#define ID_EDITCHILD1 105
#define ID_ERROR 106
#define ID_SUCCESS 107
#define ID_ARCHIVE 1008

#define MAX_LOADSTRING 100

using namespace std;

enum states
{
	start = 0, state_1 = 1, state_2 = 2, state_3 = 3, state_4 = 4,
	state_5 = 5, state_6 = 6, state_7 = 7, state_8 = 8, state_9 = 9, 
	state_10 = 10, state_11 = 11, state_12 = 12, finish = 13, archive_1 = 14, archive_2 = 15, archive_f = 16, archive_s = 17
};

enum  actions {
	unknown, clearEdit, connectDB, showError, showTable, returnTo1, startSelect,
	addItemAct, selectAll, addWhere, addCombo, addOperationAct, addOperText, selectSome,
	addLogical, finishProgram, openArchiveAct, setUP, setDOWN, returnBack, getSelect
};

enum signals {
	unknownSig = 0, selectTable = 1, selectQuery = 2,  OK = 3, error = 4, 
	success = 5, btnNext = 6, addItem = 7, addOperation = 8, exitProgram = 9, 
	openArchive = 10, btnUP = 11, btnDOWN = 12, btnEXIT = 13, setSelect = 14
};

//Таблица переходов
states changeState[18][15] = {
	/*start*/	{ state_1, start, start, start, start, start, start, start, start, start, start, start, start, start, start },
	/*state_1*/	{ state_1, state_2, state_6, state_1, state_1, state_1, state_1, state_1, state_1, finish, archive_s, state_1, state_1, state_1 },
	/*state_2*/	{ state_2, state_2, state_2, state_3, state_2, state_2, state_2, state_2, state_2, state_2, archive_s, state_2, state_2, state_2, state_2 },
	/*state_3*/	{ state_3, state_3, state_3, state_3, state_4, state_5, state_3, state_3, state_3, state_3, archive_s, state_3, state_3, state_3, state_3 },
	/*state_4*/ { state_4, state_4, state_4, state_2, state_4, state_4, state_4, state_4, state_4, state_4, archive_s, state_4, state_4, state_4, state_4 },
	/*state_5*/	{ state_5, state_5, state_5, state_1, state_5, state_5, state_5, state_5, state_5, state_5, archive_s, state_5, state_5, state_5, state_5 },
	/*state_6*/	{ state_6, state_6, state_6, state_6, state_6, state_6, state_7, state_6, state_6, state_6, archive_s, state_6, state_6, state_6, state_6 },
	/*state_7*/	{ state_7, state_7, state_7, state_8, state_7, state_7, state_9, state_7, state_7, state_7, archive_s, state_7, state_7, state_7, state_7 },
	/*state_8*/	{ state_8, state_8, state_8, state_1, state_8, state_8, state_8, state_8, state_8, state_8, archive_s, state_8, state_8, state_8, state_8 },
	/*state_9*/	{ state_9, state_9, state_9, state_9, state_9, state_9, state_9, state_10, state_9, state_9, archive_s, state_9, state_9, state_9, state_9 },
	/*state_10*/{ state_10, state_10, state_10, state_10, state_10, state_10, state_10, state_10, state_11, state_10, archive_1, state_10, state_10, state_10, state_10 },
	/*state_11*/{ state_11, state_11, state_11, state_11, state_11, state_11, state_12, state_11, state_11, state_11, archive_1, state_11, state_11, state_11, state_11 },
	/*state_12*/{ state_12, state_12, state_12, state_8, state_12, state_12, state_9, state_12, state_12, state_12, archive_1, state_12, state_12, state_12, state_12 },
	/*finish*/	{ finish, finish, finish, finish, finish, finish, finish, finish, finish, finish, finish, finish, finish, finish },

	/*archive_1*/{ archive_1, archive_1, archive_1, archive_1, archive_1, archive_1, archive_1, archive_1, archive_1, archive_1, archive_1, archive_1, archive_1, archive_f, archive_2 },
	/*archive_2*/{ archive_2, archive_2, archive_2, archive_2, archive_2, archive_2, archive_2, archive_2, archive_2, archive_2,  archive_2, archive_1, archive_1, archive_f, archive_2 },
	/*archive_f*/{ archive_f, archive_f, archive_f, archive_f, archive_f, archive_f, archive_f, archive_f, archive_f, archive_f, archive_f, archive_f, archive_f, archive_f, archive_f },
	/*archive_s*/{ archive_s, archive_s, archive_s, archive_s, archive_s, archive_s, archive_s, archive_s, archive_s, archive_s, archive_1, archive_s, archive_s, archive_s, archive_s },
};

//Таблица состояний
actions getAction[18][15] = {
	/*start*/	{unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown },
	/*state_1*/	{unknown, clearEdit, clearEdit, unknown, unknown, unknown, unknown, unknown, unknown, finishProgram, openArchiveAct, unknown, unknown, unknown, unknown },
	/*state_2*/	{unknown, unknown, unknown, connectDB, unknown, unknown, unknown, unknown, unknown, unknown, openArchiveAct, unknown, unknown, unknown, unknown },
	/*state_3*/ { unknown, unknown, unknown, unknown, showError, showTable, unknown, unknown, unknown, unknown, openArchiveAct, unknown, unknown, unknown, unknown },
	/*state_4*/	{ unknown, unknown, unknown, clearEdit, unknown, unknown, unknown, unknown, unknown, unknown, openArchiveAct, unknown, unknown, unknown, unknown },
	/*state_5*/	{ unknown, unknown, unknown, returnTo1, unknown, unknown, unknown, unknown, unknown, unknown, openArchiveAct, unknown, unknown, unknown, unknown },
	/*state_6*/	{ unknown, unknown, unknown, unknown, unknown, unknown, startSelect, unknown, unknown, unknown, openArchiveAct, unknown, unknown, unknown, unknown },
	/*state_7*/	{ unknown, unknown, unknown, selectAll, unknown, unknown, addWhere, addItemAct, unknown, unknown, openArchiveAct, unknown, unknown, unknown, unknown },
	/*state_8*/	{ unknown, unknown, unknown, returnTo1, unknown, unknown, unknown, unknown, unknown, unknown, openArchiveAct, unknown, unknown, unknown, unknown },
	/*state_9*/	{ unknown, unknown, unknown, unknown, unknown, unknown, unknown, addCombo, unknown, unknown, openArchiveAct, unknown, unknown, unknown, unknown },
	/*state_10*/{ unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, addOperationAct, unknown, openArchiveAct, unknown, unknown, unknown, unknown },
	/*state_11*/{ unknown, unknown, unknown, unknown, unknown, unknown, addOperText, unknown, unknown, unknown, openArchiveAct, unknown, unknown, unknown, unknown },
	/*state_12*/{ unknown, unknown, unknown, selectSome, unknown, unknown, addLogical, unknown, unknown, unknown, openArchiveAct, unknown, unknown, unknown, unknown },
	/*finish*/	{ unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown },
	
	/*archive_1*/{ unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, setUP, setDOWN, returnBack, getSelect },
	/*archive_2*/{ unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, setUP, setDOWN, returnBack, unknown},
	/*archive_f*/{ unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown },
	/*archive_s*/{ unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown },
};

//Таблица контекста состояний 
/*
	0 - объект меню, 1 - таблица, 2 - список с полями таблицы, 3 - список со операциями,
	4 - текстовое поле для ввода таблицы, 5 - текст.поле для ввода значения, 6 - ОК,
	7 - Далее, 8 - многострочное текстовое поле, 9 - архив, 10 - кнопка вперед, 
	11 - кнопка назад, 12 - кнопка выход, 13 - список с запросами
*/
string tableContext[18][14] = {
	/*start*/	{ "+", "", "", "", "", "", "", "", "", "+", "", "", "", "" },
	/*state_1*/	{ "+", "", "", "", "", "", "", "", "", "+", "", "", "", "" },
	/*state_2*/	{ "-", "", "", "", "+", "", "+", "", "+", "+", "", "", "", "" },
	/*state_3*/	{ "-", "", "", "", "+", "", "-", "", "-", "+", "", "", "", "" },
	/*state_4*/	{ "-", "", "", "", "-", "", "+", "", "-", "+", "", "", "", "" },
	/*state_5*/	{ "-", "+", "", "", "-", "", "+", "", "-", "+", "", "", "", "" },
	/*state_6*/	{ "-", "", "", "", "+", "", "-", "+", "+", "+", "", "", "", "" },
	/*state_7*/	{ "-", "", "+", "", "-", "", "+", "+", "+", "+", "", "", "", "" },
	/*state_8*/	{ "-", "+", "-", "", "-", "", "+", "", "-", "+", "", "", "", "" },
	/*state_9*/	{ "-", "", "+", "-", "-", "-", "-", "+", "+", "+", "", "", "", "" },
	/*state_10*/{ "-", "", "-", "+", "-", "-", "-", "+", "+", "+", "", "", "", "" },
	/*state_11*/{ "-", "", "-", "-", "-", "+", "-", "+", "+", "+", "", "", "", "" },
	/*state_12*/{ "-", "", "-", "+", "-", "", "+", "+", "+", "+", "", "", "", "" },
	/*finish*/	{ "", "", "", "", "", "", "", "", "", "+", "", "", "", "" },
	/*archive_1*/{ "-", "", "", "", "", "", "", "", "+", "-", "+", "+", "+", "+" },
	/*archive_2*/{ "-", "+", "", "", "", "", "", "", "+", "-", "+", "+", "+", "+" },
	/*archive_f*/{ "", "", "", "", "", "", "", "", "", "+", "", "", "", "" },
	/*archive_s*/{ "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
};
enum states current_state = start;
enum signals current_signal = unknownSig;
enum actions current_action;

enum states callback_state;

wstring selects[1024];
int cnt_selects = 0;

HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];

BOOL RegClass(WNDPROC Proc, LPCTSTR szName, UINT brBackground);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
HWND CreateListView(HWND hWndParent, UINT uId);
void getSignal(int lowordId, int hiwordId);
BOOL CreateMenuItem(HMENU hMenu, char *str, UINT ulns, UINT uCom, HMENU hSubMenu, BOOL flag, UINT fType);
void changeContext(states state);
void funcSelect(string query);

sqlite3* dbConnection;
wstring errorMsg;
bool first = true;
int colCount;

HMENU hMenu, hModeMenu;
HWND hArchive;
HWND hEditControlFormula;
HWND hListFields;
HWND allSelects;

HWND hSelect;
HWND hBtnOK;
HWND hBtnNext;
HWND hBtnUP;
HWND hBtnDOWN;
HWND hBtnEXIT;

HWND hwndResTable;
HWND hOperations;
HWND hTextSelect;

wstring operations[8] = { TEXT("="), TEXT(">"), TEXT("<"), TEXT(">="), TEXT("<="), TEXT("!="), TEXT("AND"), TEXT("OR") };

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	hInst = hInstance;

	if (!RegClass(WndProc, L"Main", COLOR_GRADIENTACTIVECAPTION)) { return FALSE; };

	HWND hWnd = CreateWindowW(L"Main", L"Main", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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
BOOL CreateMenuItem(HMENU hMenu, char* str, UINT ulns, UINT uCom, HMENU hSubMenu, BOOL flag, UINT fType)
{
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE | MIIM_TYPE | MIIM_SUBMENU | MIIM_ID;
	mii.fType = fType;
	mii.fState = MFS_ENABLED;
	mii.dwTypeData = (LPWSTR)str;
	mii.cch = sizeof(str);
	mii.wID = uCom;
	mii.hSubMenu = hSubMenu;
	return InsertMenuItem(hMenu, ulns, flag, &mii);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


	switch (message)
	{
	case WM_CREATE:
	{
		hMenu = CreateMenu();
		hModeMenu = CreatePopupMenu();

		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hModeMenu, L"Выбрать режим");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, ID_ARCHIVE, L"Архив");
		AppendMenu(hModeMenu, MF_STRING, ID_TABLEMENU, L"Режим таблицы");
		AppendMenu(hModeMenu, MF_STRING, ID_SELECTMENU, L"Режим запроса");

		SetMenu(hWnd, hMenu);
		SetMenu(hWnd, hModeMenu);

		hEditControlFormula = CreateWindowEx(0, L"EDIT",   // predefined class 
			L"Введите таблицу",         // no window title 
			WS_CHILD | ES_LEFT | WS_BORDER,
			30, 70, 150, 20,   // set size in WM_SIZE message 
			hWnd,         // parent window 
			(HMENU)ID_EDITCHILD,   // edit control ID 
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);

		hListFields = CreateWindowEx(0, L"listbox", NULL,
			WS_CHILD | LBS_STANDARD| CBS_AUTOHSCROLL|WS_VSCROLL,
			210, 30,
			150, 200,
			hWnd,
			(HMENU)IDC_LISTVIEW,
			hInst, NULL);

		hSelect = CreateWindowEx(0, L"EDIT", NULL,
			WS_CHILD |  WS_VSCROLL |
			ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
			400, 30,
			200, 200,
			hWnd, nullptr, hInst, NULL);

		hBtnOK = CreateWindowEx(0, L"button", L"OK",
			WS_CHILD | BS_PUSHBUTTON,
			650, 60,
			50, 30,
			hWnd, (HMENU)IDC_BTNOK, hInst, NULL);

		hBtnNext = CreateWindowEx(0, L"button", L"Далее",
			WS_CHILD | BS_PUSHBUTTON,
			650, 100,
			70, 30,
			hWnd, (HMENU)IDC_BTNNEXT, hInst, NULL);

		hBtnUP = CreateWindowEx(0, L"button", L"Вперёд",
			WS_CHILD | BS_PUSHBUTTON,
			650, 60,
			70, 30,
			hWnd, (HMENU)IDC_BTNUP, hInst, NULL);

		hBtnDOWN = CreateWindowEx(0, L"button", L"Назад",
			WS_CHILD | BS_PUSHBUTTON,
			650, 100,
			70, 30,
			hWnd, (HMENU)IDC_BTNDOWN, hInst, NULL);

		hBtnEXIT = CreateWindowEx(0, L"button", L"Выход",
			WS_CHILD | BS_PUSHBUTTON,
			650, 190,
			70, 30,
			hWnd, (HMENU)IDC_BTNEXIT, hInst, NULL);

		hwndResTable = CreateListView(hWnd, ID_TABLE);

		hOperations = CreateWindowEx(WS_EX_STATICEDGE, L"COMBOBOX", L"MyCombo2",
			CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL,
			630, 150, 150, 100, hWnd, (HMENU)ID_COMBOBOX, hInst, NULL);

		hTextSelect = CreateWindowEx(0, L"EDIT",   // predefined class 
			L"Введите значение",         // no window title 
			WS_CHILD | ES_LEFT | WS_BORDER,
			630, 200, 150, 20,   // set size in WM_SIZE message 
			hWnd,         // parent window 
			(HMENU)ID_EDITCHILD1,   // edit control ID 
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);
		EnableWindow(hTextSelect, FALSE);

		hArchive = CreateWindowEx(0, L"listbox", NULL,
			WS_CHILD | LBS_COMBOBOX| CBS_AUTOHSCROLL | WS_VSCROLL| LBS_NOTIFY,
			30, 30,
			300, 200,
			hWnd,
			(HMENU)IDC_LISTVIEW1,
			hInst, NULL);

	}
	break;
	case WM_COMMAND:
	{
		int lowordId = LOWORD(wParam);
		int hiwordId = HIWORD(wParam);
		
		getSignal(lowordId, hiwordId);
		current_action = getAction[current_state][current_signal];
		switch (current_action) {
			case connectDB:
			{
				sqlite3_stmt *res;
				if (sqlite3_open("test.db", &dbConnection)) {
					PostMessage(hWnd, WM_COMMAND, ID_ERROR, hiwordId);
					sqlite3_close(dbConnection);
					break;
				}
				else {
					string query = "SELECT * FROM ";
					wchar_t table[64];
					GetWindowText(hEditControlFormula, table, 64);
					wstring ws(table);
					string str(ws.begin(), ws.end());
					query += str;

					int rc = sqlite3_prepare_v2(dbConnection, query.c_str(), -1, &res, 0);

					if (rc != SQLITE_OK) {
						errorMsg = static_cast<const wchar_t*>(sqlite3_errmsg16(dbConnection));
						sqlite3_finalize(res);
						sqlite3_close(dbConnection);
						PostMessage(hWnd, WM_COMMAND, ID_ERROR, hiwordId);
					} else
						PostMessage(hWnd, WM_COMMAND, ID_SUCCESS, hiwordId);
				}
			}
				break;
			case clearEdit:
			{
				PostMessage(hEditControlFormula, EM_SETSEL, 0, -1);
				PostMessage(hEditControlFormula, WM_CLEAR, 0, 0);
			}
				break;
			case showError:
			{
				if(MessageBox(hWnd, errorMsg.c_str(), L"Error", MB_OK | MB_ICONERROR) == IDOK)
					PostMessage(hWnd, WM_COMMAND, IDC_BTNOK, hiwordId);
			}
				break;
			case showTable:
			{
				sqlite3_stmt *res;
				const wchar_t* result;
				string query = "SELECT * FROM ";
				wchar_t table[64];
				GetWindowText(hEditControlFormula, table, 64);

				wstring ws(table);
				string str(ws.begin(), ws.end());
				query += str;

				wchar_t text[1024] = L"SELECT * FROM ";
				wstring t(text);
				t += ws;
				t += L";";
				SetWindowText(hSelect, t.c_str());

				selects[cnt_selects] = t;
				cnt_selects++;
				
				funcSelect(query);
			}
			break;
			case returnTo1: 
			{
				for (int i = colCount - 1; i >= 0; i--)
					ListView_DeleteColumn(hwndResTable, i);
				ListView_DeleteAllItems(hwndResTable);
				PostMessage(hListFields, LB_RESETCONTENT, 0, 0);
				PostMessage(hSelect, EM_SETSEL, 0, -1);
				PostMessage(hSelect, WM_CLEAR, 0, 0);
			}
				break;
			case startSelect: 
			{
				sqlite3_stmt *res;
				const wchar_t* result;
				string query = "SELECT * FROM ";
				wchar_t table[64];
				GetWindowText(hEditControlFormula, table, 64);
				wstring ws(table);
				string str(ws.begin(), ws.end());
				query += str;

				sqlite3_open("test.db", &dbConnection);

				int rc = sqlite3_prepare_v2(dbConnection, query.c_str(), -1, &res, 0);
				rc = sqlite3_step(res);

				if (rc == SQLITE_ROW) {
					colCount = sqlite3_data_count(res);
					for (int i = 0; i < colCount; i++)
					{
						result = static_cast<const wchar_t*>(sqlite3_column_name16(res, i));
						int pos = (int)SendMessage(hListFields, LB_ADDSTRING, 0,
							(LPARAM)result);
						SendMessage(hListFields, LB_SETITEMDATA, pos, (LPARAM)i);
					}
				}

				SetWindowText(hSelect, L"SELECT ");
				PostMessage(hTextSelect, EM_SETSEL, 0, -1);
				PostMessage(hTextSelect, WM_CLEAR, 0, 0);

			}
				break;
			case addItemAct:
			{
				DWORD dwSel = SendMessage(hListFields, LB_GETCURSEL, 0, wParam);
				if (dwSel != LB_ERR)
				{
					TCHAR buff[10] = TEXT("");
					SendMessage(hListFields, LB_GETTEXT, (WPARAM)dwSel, (LPARAM)buff);

					wchar_t selectText[1024];
					GetWindowText(hSelect, selectText, 1024);

					wstring str(buff);
					wstring ws(selectText);

					if (first) first = !first;
					else ws += L", ";
					ws += str;
					SetWindowText(hSelect, ws.c_str());
				}
				
			}
				break;
			case selectAll:
			{
				
				wchar_t selectText[1024];
				GetWindowText(hSelect, selectText, 1024);
				wstring ws(selectText);
				ws += L" FROM ";
				wchar_t table[64];
				GetWindowText(hEditControlFormula, table, 64);
				ws += wstring(table);
				ws += L";";
				SetWindowText(hSelect, ws.c_str());
				string query(ws.begin(), ws.end());
				selects[cnt_selects] = ws;
				cnt_selects++;

				funcSelect(query);
				
			}
				break;
			case addWhere:
			{
				wchar_t selectText[1024];
				GetWindowText(hSelect, selectText, 1024);
				wstring ws(selectText);
				ws += L" FROM ";
				wchar_t table[64];
				GetWindowText(hEditControlFormula, table, 64);
				ws += wstring(table);
				ws += L" WHERE ";
				SetWindowText(hSelect, ws.c_str());
				first =  true;
			}
				break;
			case addCombo:
			{
				DWORD dwSel = SendMessage(hListFields, LB_GETCURSEL, 0, wParam);
				if (dwSel != LB_ERR)
				{
					TCHAR buff[10] = TEXT("");
					SendMessage(hListFields, LB_GETTEXT, (WPARAM)dwSel, (LPARAM)buff);

					wchar_t selectText[1024];
					GetWindowText(hSelect, selectText, 1024);

					wstring str(buff);
					wstring ws(selectText);

					ws += str;
					SetWindowText(hSelect, ws.c_str());
				}
				for (int i = 0; i < 6; i++)
					ComboBox_AddString(hOperations, operations[i].c_str());
			}
			case addOperationAct:
			{
				first = true;
				int id = SendMessage(hOperations, CB_GETCURSEL, 0, 0);
				if (id != CB_ERR)
				{
					wchar_t selectText[1024];
					GetWindowText(hSelect, selectText, 1024);
					wstring str(operations[id]);
					wstring ws(selectText);

					ws += str;
					SetWindowText(hSelect, ws.c_str());
					ComboBox_ResetContent(hOperations);
				}
			}
				break;
			case addOperText:
			{
				wchar_t value[64];
				GetWindowText(hTextSelect, value, 64);
				wchar_t selectText[1024];
				GetWindowText(hSelect, selectText, 1024);
				wstring str(value);
				wstring ws(selectText);
				ws += L"'";
				ws += str;
				ws += L"' ";
				SetWindowText(hSelect, ws.c_str());
				for (int i = 6; i < 8; i++)
					ComboBox_AddString(hOperations, operations[i].c_str());
			}
				break;
			case selectSome:
			{
				ComboBox_ResetContent(hOperations);
				wchar_t selectText[1024];
				GetWindowText(hSelect, selectText, 1024);
				wstring ws(selectText);
				string query(ws.begin(), ws.end());

				selects[cnt_selects] = ws;
				cnt_selects++;

				funcSelect(query);
			}
				break;
			case addLogical: 
			{
				int id = SendMessage(hOperations, CB_GETCURSEL, 0, 0);
				if (id != CB_ERR)
				{
					wchar_t selectText[1024];
					GetWindowText(hSelect, selectText, 1024);
					wstring str(operations[id + 6]);
					wstring ws(selectText);

					ws += str;
					ws += L" ";
					SetWindowText(hSelect, ws.c_str());
					ComboBox_ResetContent(hOperations);
				}
				PostMessage(hTextSelect, EM_SETSEL, 0, -1);
				PostMessage(hTextSelect, WM_CLEAR, 0, 0);
			}
				break;
			case finishProgram:
				PostQuitMessage(0);
				break;
			case openArchiveAct:
			{
				callback_state = current_state;
				
				for (int i = 0; i < cnt_selects; i++)
				{
					ListBox_AddString(hArchive, selects[i].c_str());
				}
				SetWindowText(hSelect, selects[0].c_str());
				ListBox_SetCurSel(hArchive, 0);
				PostMessage(hWnd, WM_COMMAND, ID_ARCHIVE, hiwordId);

			}
				break;
			case setUP:
			{
				int pos = ListBox_GetCurSel(hArchive);
				if (pos < ListBox_GetCount(hArchive) - 1)
				{
					ListBox_SetCurSel(hArchive, pos + 1);
					SetWindowText(hSelect, selects[pos + 1].c_str());
				}
			}
				break;
			case setDOWN:
			{
				int pos = ListBox_GetCurSel(hArchive);
				if (pos > 0)
				{
					ListBox_SetCurSel(hArchive, pos - 1);
					SetWindowText(hSelect, selects[pos - 1].c_str());
				}
			}
			break;
			case returnBack:
			{
				for (int i = colCount - 1; i >= 0; i--)
					ListView_DeleteColumn(hwndResTable, i);
				ListView_DeleteAllItems(hwndResTable);
				PostMessage(hListFields, LB_RESETCONTENT, 0, 0);
				PostMessage(hArchive, LB_RESETCONTENT, 0, 0);
				PostMessage(hSelect, EM_SETSEL, 0, -1);
				PostMessage(hSelect, WM_CLEAR, 0, 0);
				current_state = callback_state;
			}
			break;
			case getSelect:
			{
				for (int i = colCount - 1; i >= 0; i--)
					ListView_DeleteColumn(hwndResTable, i);
				ListView_DeleteAllItems(hwndResTable);
				int pos = ListBox_GetCurSel(hArchive);
				SetWindowText(hSelect, selects[pos].c_str());
				string query(selects[pos].begin(), selects[pos].end());
				funcSelect(query);
			}
				break;
			default: break;
		}
		current_state = changeState[current_state][current_signal];
		changeContext(current_state);

	}
	break;
	case WM_DESTROY:
		sqlite3_close(dbConnection);
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		SendMessage(hWnd, WM_COMMAND, ID_EXIT, 0);
		break;
	default:
		sqlite3_close(dbConnection);
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

HWND CreateListView(HWND hWndParent, UINT uId)
{
	INITCOMMONCONTROLSEX icex;
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	RECT rcClient;
	GetClientRect(hWndParent, &rcClient);

	HWND hWndLV = CreateWindowEx(NULL, WC_LISTVIEW, NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_REPORT | LVS_EDITLABELS | LVS_EX_FULLROWSELECT,
		30, 250,
		600, 300,
		hWndParent, (HMENU)uId, hInst, NULL);

	ListView_SetExtendedListViewStyleEx(hWndLV, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	return (hWndLV);
}

void getSignal(int lowordId, int hiwordId) {
	switch (lowordId) {
		case ID_TABLEMENU:
			current_signal = selectTable;
		break;
		case ID_SELECTMENU:
			current_signal = selectQuery;
			break;
		case IDC_BTNOK:
			current_signal = OK;
			break;
		case IDC_BTNNEXT:
			current_signal = btnNext;
			if (current_state == state_12)
				if (SendMessage(hOperations, CB_GETCURSEL, 0, 0) != CB_ERR)
					current_signal = btnNext;
				else current_signal = unknownSig;

			break;
		case ID_ERROR:
			current_signal = error;
			break;
		case ID_SUCCESS:
			current_signal = success;
			break;
		case IDC_LISTVIEW:
			switch (hiwordId) {
				case LBN_DBLCLK:
					current_signal = addItem;
					break;
				default:
					current_signal = unknownSig;
			}
			break;
		case ID_COMBOBOX:
			switch (hiwordId) {
			case CBN_SELCHANGE:
				current_signal = addOperation;
				break;
			default:
				current_signal = unknownSig;
			}
			break;

		case ID_EXIT: 
			current_signal = exitProgram;
			break;
		case ID_ARCHIVE:
			current_signal = openArchive;
			break;

		case IDC_LISTVIEW1:
			switch (hiwordId) {
			case LBN_DBLCLK:
				current_signal = setSelect;
				break;
			default:
				current_signal = unknownSig;
				break;
			}
			break;
		case IDC_BTNUP:
			current_signal = btnUP;
			break;

		case IDC_BTNDOWN:
			current_signal = btnDOWN;
			break;
		case IDC_BTNEXIT:
			current_signal = btnEXIT;
			break;
		default:
			current_signal = unknownSig;
			break;
	}
}


void changeContext(states state)
{
	EnableMenuItem(hMenu, ID_TABLEMENU, tableContext[state][0] == "+" ? MF_ENABLED : MF_DISABLED);
	EnableMenuItem(hMenu, ID_SELECTMENU, tableContext[state][0] == "+" ? MF_ENABLED : MF_DISABLED);
	EnableMenuItem(hMenu, ID_EXIT, tableContext[state][0] == "+" ? MF_ENABLED : MF_DISABLED);

	ShowWindow(hwndResTable, tableContext[state][1] != "" ? SW_SHOW : SW_HIDE);
	EnableWindow(hwndResTable, tableContext[state][1] == "+" ? true : false);


	ShowWindow(hListFields, tableContext[state][2] != "" ? SW_SHOW : SW_HIDE);
	EnableWindow(hListFields, tableContext[state][2] == "+" ? true : false);

	ShowWindow(hOperations, tableContext[state][3] != "" ? SW_SHOW : SW_HIDE);
	EnableWindow(hOperations, tableContext[state][3] == "+" ? true : false);

	ShowWindow(hEditControlFormula, tableContext[state][4] != "" ? SW_SHOW : SW_HIDE);
	EnableWindow(hEditControlFormula, tableContext[state][4] == "+" ? true : false);

	ShowWindow(hTextSelect, tableContext[state][5] != "" ? SW_SHOW : SW_HIDE);
	EnableWindow(hTextSelect, tableContext[state][5] == "+" ? true : false);

	ShowWindow(hBtnOK, tableContext[state][6] != "" ? SW_SHOW : SW_HIDE);
	EnableWindow(hBtnOK, tableContext[state][6] == "+" ? true : false);

	ShowWindow(hBtnNext, tableContext[state][7] != "" ? SW_SHOW : SW_HIDE);
	EnableWindow(hBtnNext, tableContext[state][7] == "+" ? true : false);

	ShowWindow(hSelect, tableContext[state][8] != "" ? SW_SHOW : SW_HIDE);
	EnableWindow(hSelect, tableContext[state][8] == "+" ? true : false);

	EnableMenuItem(hMenu, ID_ARCHIVE, tableContext[state][9] == "+" ? MF_ENABLED : MF_DISABLED);

	ShowWindow(hBtnUP, tableContext[state][10] != "" ? SW_SHOW : SW_HIDE);
	EnableWindow(hBtnUP, tableContext[state][10] == "+" ? true : false);

	ShowWindow(hBtnDOWN, tableContext[state][11] != "" ? SW_SHOW : SW_HIDE);
	EnableWindow(hBtnDOWN, tableContext[state][11] == "+" ? true : false);

	ShowWindow(hBtnEXIT, tableContext[state][12] != "" ? SW_SHOW : SW_HIDE);
	EnableWindow(hBtnEXIT, tableContext[state][12] == "+" ? true : false);

	ShowWindow(hArchive, tableContext[state][13] != "" ? SW_SHOW : SW_HIDE);
	EnableWindow(hArchive, tableContext[state][13] == "+" ? true : false);
	InvalidateRect(GetParent(hSelect), NULL, TRUE);
}

void funcSelect(string query)
{
	sqlite3_stmt *res;
	const wchar_t* result;
	int rc = sqlite3_prepare_v2(dbConnection, query.c_str(), -1, &res, 0);
	rc = sqlite3_step(res);

	//Структура для создания столбцов
	LVCOLUMN structColumn;
	memset(&structColumn, 0, sizeof(structColumn));
	structColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	structColumn.fmt = LVCFMT_LEFT;
	structColumn.cx = 100;

	if (rc == SQLITE_ROW) {
		colCount = sqlite3_data_count(res);
		for (int i = 0; i < colCount; i++)
		{
			result = static_cast<const wchar_t*>(sqlite3_column_name16(res, i));
			structColumn.pszText = (LPWSTR)result;
			ListView_InsertColumn(hwndResTable, i, &structColumn);
		}
	}

	rc = sqlite3_prepare_v2(dbConnection, query.c_str(), -1, &res, 0);
	int row = 0;
	while (sqlite3_step(res) == SQLITE_ROW) {
		LVITEM item;
		memset(&item, 0, sizeof(LVITEM));
		item.mask = LVIF_TEXT;
		item.iItem = row;
		item.iSubItem = 0;
		int list = ListView_InsertItem(hwndResTable, &item);
		for (int i = 0; i < colCount; i++)
		{
			result = static_cast<const wchar_t*>(sqlite3_column_text16(res, i));
			ListView_SetItemText(hwndResTable, list, i, (LPWSTR)result);
		}
		row++;
	}
	sqlite3_finalize(res);
	sqlite3_close(dbConnection);
	first = true;
}
