#pragma once
#include <ObjIdl.h>
#include <gdiplus.h>
using namespace Gdiplus;

class GUIEx
{
public:
	void createGUI(Graphics* graphics);

	void drawString(Color color, const WCHAR* str, float x, float y);
	void drawLine(Color color, float sx, float sy, float ex, float ey, bool rect = true);
	void drawRect(Color color, float x, float y, float width, float height);
	void drawCircle(Color color, float x, float y, float radius, int accuracy);
	
	bool button(Color color, const WCHAR* str, float x, float y, float width, float height);

private:
	Graphics* _graphics = nullptr;
	Gdiplus::StringFormat _centerFormat;
	int _windowX = 0;
	int _windowY = 0;

	Font* _font = nullptr;
};


