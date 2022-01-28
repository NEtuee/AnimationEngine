#pragma once
#include <windows.h>

enum class MessageState : short
{
	M_QUIT = -1,
	M_PEEK,
	M_FALSE,
};

class WindowApp
{
public:
								WindowApp(int width, int height);
								~WindowApp();

	bool						createWindowApp();
	void						destroyWindowApp();
	void						initialize(int width, int height);

	MSG*						getMessagePointer();
	MessageState				peekMessageCheck();

	inline const HWND			getHWND() { return _hWnd; }
	inline const HINSTANCE		getInstance() { return _instance; }
	inline const MSG&			getMessage() { return _message; }
	inline const MessageState	getRecentlyState() { return _recentlyState; }
	inline const int			getWidth() { return _windowWidth; }
	inline const int			getHeight() { return _windowHeight; }
private:
	HWND			_hWnd;
	HINSTANCE		_instance;
	MSG				_message;
	MessageState	_recentlyState;
	int				_windowWidth;
	int				_windowHeight;
};

