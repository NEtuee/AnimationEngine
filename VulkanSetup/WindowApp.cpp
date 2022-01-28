#include "WindowApp.h"

TCHAR szWndAppName[] = TEXT("WindowMain");
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

WindowApp::WindowApp(int width, int height)
	:_hWnd(0), _instance(0), _recentlyState(MessageState::M_FALSE)
{
	initialize(width, height);
	createWindowApp();
}

WindowApp::~WindowApp()
{
	destroyWindowApp();
}

bool WindowApp::createWindowApp()
{
	WNDCLASS wc = { 0 };
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szWndAppName;

	if (!RegisterClass(&wc))
		return false;

	_hWnd = CreateWindow(szWndAppName, szWndAppName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		_windowWidth, _windowHeight,
		NULL, NULL, NULL, NULL);


	//32bit = GWL_HINSTANCE 64bit = GWLP_HINSTANCE
	_instance = reinterpret_cast<HINSTANCE>(GetWindowLong(_hWnd, GWLP_HINSTANCE));

	return ShowWindow(_hWnd, true);
}

void WindowApp::destroyWindowApp()
{
}

void WindowApp::initialize(int width, int height)
{
	_windowWidth = width;
	_windowHeight = height;
}

MSG* WindowApp::getMessagePointer()
{
	return &_message;
}

MessageState WindowApp::peekMessageCheck()
{
	if (PeekMessage(&_message, NULL, 0, 0, PM_REMOVE))
	{
		if (_message.message == WM_QUIT)
		{
			_recentlyState = MessageState::M_QUIT;
			return MessageState::M_QUIT;
		}
		else
		{
			TranslateMessage(&_message);
			DispatchMessage(&_message);
		}

		_recentlyState = MessageState::M_PEEK;
		return MessageState::M_PEEK;
	}

	_recentlyState = MessageState::M_FALSE;
	return MessageState::M_FALSE;
}

extern LRESULT ImGui_ImplDX11_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplDX11_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}