#include <windows.h>
#include <iostream>

LRESULT WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_CREATE: {
		RECT rc{};
		GetClientRect(hwnd, &rc);

		CreateWindow(
			TEXT("STATIC"), // system class
			TEXT("moamen"),
			WS_CHILDWINDOW | WS_BORDER | WS_VISIBLE,
			rc.right / 2, rc.bottom / 2, rc.right / 2, rc.bottom / 2,
			hwnd,
			NULL,
			GetModuleHandle(NULL),
			NULL);
		return 0;
	}

	case WM_WINDOWPOSCHANGED: {
		EnumChildWindows(hwnd,
			[](HWND childHwnd, LPARAM lParam) -> BOOL {
				WINDOWPOS* wp = (WINDOWPOS*)lParam;

				MoveWindow(childHwnd, 
					wp->cx / 2 - wp->cx / 4, wp->cy / 2 - wp->cy / 4,
					wp->cx / 2, wp->cy / 2,
					FALSE);
				return TRUE;
			},
			lParam);
		return 0;
	}

	case WM_GETMINMAXINFO: {
		MINMAXINFO* mmi = (MINMAXINFO*)lParam;
		mmi->ptMinTrackSize = POINT{600, 450};
		return 0;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE _, PSTR szCmd, int nCmd) {
	// application local class
	const TCHAR* MAIN_WINDOW{ TEXT("Main") };
	
	WNDCLASSEX wcx{};

	wcx.cbSize = sizeof(wcx);
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcx.lpszClassName = MAIN_WINDOW;
	wcx.hInstance = hInstance;
	wcx.lpfnWndProc = WinProc;
	wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	if (!RegisterClassEx(&wcx)) {
		MessageBox(NULL, TEXT("COULDN'T REGISTER WINDOW CLASS"), TEXT(""), MB_OK);
		return 1;
	}


	HWND hwnd = CreateWindow(
		MAIN_WINDOW, 
		TEXT("moamen"), 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
		CW_USEDEFAULT, CW_USEDEFAULT, 
		CW_USEDEFAULT, CW_USEDEFAULT, 
		NULL, 
		NULL, 
		hInstance, 
		NULL);

	if (!hwnd) {
		MessageBox(NULL, TEXT("COULDN'T REGISTER WINDOW CLASS"), TEXT(""), MB_OK);
		return 1;
	}


	MSG msg{};
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}