#include <windows.h>
#include <string>
#include <cstdio>
#include <TCHAR.h>

BOOL RegClass(WNDPROC, LPCTSTR, UINT);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndChild(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInstance;
char szMainClass[] = "MainClass";
char szChildClass[] = "ChildClass";
char szTitle[] = "Test";
//HWND child_window = nullptr;
HWND hwnd;
HWND hwnd1;

int x, y, newX, newY, width, height;
RECT client_rect;
RECT child_rect;
void GetErrorMessage(){
	LPVOID ru, en;
	char buffer[64];

	DWORD error = GetLastError();
	if (error == 0)
		return;
  	DWORD dwRu = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                  NULL, error, MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
                  (LPTSTR)&ru, 0, NULL);
    DWORD dwEn = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                  NULL, error, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
                  (LPTSTR)&en, 0, NULL);

    sprintf(buffer, "Error code: %d\n",  error);
    strcat(buffer, (LPTSTR)ru);
   	strcat(buffer, (LPTSTR)en);
  	MessageBox(NULL, buffer, TEXT("Error"), MB_ICONHAND);
  	LocalFree(ru);
  	LocalFree(en);
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR IpszCmdLine,
	int nCmdShow)
{
	MSG msg;
	hInstance=hInst;
	if (!RegClass(WndProc, szMainClass, COLOR_WINDOW))
		return FALSE;
	//Ошибка регистрации класса: регистрируем уже существующий класс
	RegClass(WndProc, szMainClass, COLOR_WINDOW);
	if (!RegClass(WndChild, szChildClass, COLOR_BTNFACE))
		return FALSE;
	GetErrorMessage();
	hwnd = CreateWindow(szMainClass, szTitle,
		WS_OVERLAPPEDWINDOW|WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, 
		0, 0, hInstance, NULL);
	if (!hwnd) return FALSE;
	while (GetMessage(&msg, 0, 0, 0)) DispatchMessage(&msg);
	return msg.wParam;
}

BOOL RegClass(WNDPROC Proc, LPCTSTR szName, UINT brBackground)
{
	WNDCLASS wc;
	wc.style = wc.cbClsExtra=wc.cbWndExtra = 0;
	wc.lpfnWndProc = Proc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground=(HBRUSH)(brBackground + 1);
	wc.lpszMenuName = (LPCTSTR)NULL;
	wc.lpszClassName = szName;
	return (RegisterClass(&wc) !=0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam,
	LPARAM lParam)
{
	GetClientRect(hwnd,&client_rect);
	width = client_rect.right - client_rect.left;
	height = client_rect.bottom - client_rect.top;
	LPVOID ru;

	switch(msg){
		case WM_DESTROY: 
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_LBUTTONDOWN:

		{
			HWND child_window;
			x = LOWORD(lParam); //узнаём координаты
			y = HIWORD(lParam);

			if (IsWindow(child_window))
				SendMessage(child_window,WM_CLOSE,0,0);

				//Незарегестрированный класс окна
			
			child_window = CreateWindow(szChildClass, "Coordinates", WS_CHILD|WS_SYSMENU|WS_CAPTION|WS_VISIBLE,
				x, y, 210, 100, child_window, 0, 0, NULL);
			GetErrorMessage();
						        
			break;
		}
		
		case WM_RBUTTONDOWN:
		{
			int delX, delY, winX, winY;
			HWND child_window;
			newX = LOWORD(lParam); //узнаём координаты
			newY = HIWORD(lParam);

			if (IsWindow(child_window)){
				if (newX <= (width / 2) && newY <= (height / 2))
				{			
					delX = abs(client_rect.left - newX);
					delY = abs(client_rect.top - newY);
					winX = abs(client_rect.right - delX) - 105;
					winY = abs(client_rect.bottom - delY) - 50;
				}
				else if (newX <= width / 2 && newY > height / 2)
				{
					delX = abs(client_rect.left - newX);
					delY = abs(client_rect.bottom - newY);
					winX = abs(client_rect.right - delX) - 105;
					winY = abs(client_rect.top - delY) - 50;
				}
				else if (newX > width / 2 && newY <= height / 2)
				{
					delX = abs(client_rect.right - newX);
					delY = abs(client_rect.top - newY);
					winX = abs(client_rect.left - delX) - 105;
					winY = abs(client_rect.bottom - delY) - 50;				
				}
				else if (newX > width / 2 && newY > height / 2)
				{
					delX = abs(client_rect.right - newX);
					delY = abs(client_rect.bottom - newY);
					winX = abs(client_rect.left - delX) - 105;
					winY = abs(client_rect.top - delY) - 50;
				}
			}

			//MoveWindow(child_window, winX, winY, 210, 100, true);
			//Пользователь пытается двигать окно, которого не существует
			GetErrorMessage();
			break;
		}
case WM_SYSCOMMAND:{            
	if (GetMenuState(0,0,0)!=0xFFFFFFFF)
            MessageBox(NULL,"clicked","hey",MB_OK);
		GetErrorMessage();

			break;
		}
 

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
