#include "GUIEx.h"
#include "../VulkanSetup/Input.h"
#include <corecrt_math.h>

void GUIEx::createGUI(Graphics* graphics)
{
	_graphics = graphics;
	FontFamily fontFamily(L"Times New Roman");
	_font = new Font(&fontFamily, 10, FontStyleRegular, UnitPixel);

	_centerFormat.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
	_centerFormat.SetAlignment(StringAlignment::StringAlignmentCenter);
}

void GUIEx::drawString(Color color, const WCHAR* str, float x, float y)
{
	SolidBrush  solidBrush(color);
	_graphics->DrawString(str, -1, _font, PointF(x, y), &solidBrush);
}

void GUIEx::drawLine(Color color, float sx, float sy, float ex, float ey, bool rect)
{
	Pen thinpen(color, 2);

	_graphics->DrawLine(&thinpen, sx, sy, ex, ey);
	if (rect)
	{
		drawRect(color, sx, sy, 4, 4);
		drawRect(color, ex, ey, 4, 4);
		//graphics->DrawRectangle(&thinpen, (INT)start.x - 2, (INT)start.y - 2, 4, 4);
		//graphics->DrawRectangle(&thinpen, (INT)end.x - 2, (INT)end.y - 2, 4, 4);
	}
}

void GUIEx::drawRect(Color color, float x, float y, float width, float height)
{
	Pen thinpen(color, 2);
	_graphics->DrawRectangle(&thinpen, x - width * 0.5f, y - height * 0.5f, width, height);
}

void GUIEx::drawCircle(Color color, float x, float y, float radius, int accuracy)
{
	float factor = 360.f / (float)accuracy;
	float timer = 0.f;
	for (int i = 0; i < accuracy; ++i)
	{
		float sx = (cosf((i * factor) * 3.141592f / 180.f)) * radius + x;
		float sy = (sinf((i * factor) * 3.141592f / 180.f)) * radius + y;

		float ex = (cosf(((i + 1) * factor) * 3.141592f / 180.f)) * radius + x;
		float ey = (sinf(((i + 1) * factor) * 3.141592f / 180.f)) * radius + y;

		drawLine(color, sx, sy, ex, ey, false);
	}
}

bool GUIEx::button(Color color, const WCHAR* str, float x, float y, float width, float height)
{
	SolidBrush  solidBrush(color);
	RectF colRec(x - width * 0.5f, y - height * 0.5f, width, height);
	_graphics->DrawString(str, -1, _font, colRec, &_centerFormat, &solidBrush);
	drawRect(color, x, y, width, height);

	if (colRec.Contains(PointF(Input::getInstance()->getMouseX(), Input::getInstance()->getMouseY())))
	{
		return Input::getInstance()->isDown(VK_LBUTTON);
	}
	
	return false;
}
