#include "Coordinate.h"
#include "Math.h"
#include <string>

void Coordinate::createCoordinate(Color color, int centerX, int centerY, int windowX, int windowY)
{
	_color = color;
	_centerX = centerX;
	_centerY = centerY;
	_windowX = windowX;
	_windowY = windowY;

	setRatio(10, 10);

	FontFamily fontFamily(L"Times New Roman");
	_font = new Font(&fontFamily, 12, FontStyleRegular, UnitPixel);
	PointF pointF(30.f, 10.f);
}

void Coordinate::setRatio(int x, int y)
{
	_ratioX = x;
	_ratioY = y;
}

void Coordinate::addRatioX(int value)
{
	_ratioX += value;
	_ratioX = _ratioX <= 1 ? 2 : _ratioX;
}

void Coordinate::addRatioY(int value)
{
	_ratioY += value;
	_ratioY = _ratioY <= 1 ? 2 : _ratioY;
}

void Coordinate::addCenterX(int value)
{
	_centerX += value;
}

void Coordinate::addCenterY(int value)
{
	_centerY += value;
}

XMFLOAT2 Coordinate::toCoordinateSpace(XMFLOAT2 world)
{
	return XMFLOAT2(world.x * _ratioX + _centerX, world.y * -_ratioY + _centerY);
}

XMFLOAT2 Coordinate::toWorldSpace(XMFLOAT2 coord)
{
	return XMFLOAT2((coord.x - _centerX) / _ratioX, (coord.y - _centerY) / -_ratioY);
}

void Coordinate::drawString(Graphics* graphics, Color color, const WCHAR* str, float x, float y)
{
	SolidBrush  solidBrush(color);
	graphics->DrawString(str, -1, _font, PointF(x,y), &solidBrush);
}

void Coordinate::drawLine(Graphics* graphics, Color color, float sx, float sy, float ex, float ey, bool rect)
{
	Pen thinpen(color, 2);

	XMFLOAT2 start = toCoordinateSpace(XMFLOAT2{ sx,sy });
	XMFLOAT2 end = toCoordinateSpace(XMFLOAT2{ ex,ey });

	graphics->DrawLine(&thinpen, start.x, start.y, end.x, end.y);
	if (rect)
	{
		drawRect(graphics, color, sx, sy, 4, 4);
		drawRect(graphics, color, ex, ey, 4, 4);
		//graphics->DrawRectangle(&thinpen, (INT)start.x - 2, (INT)start.y - 2, 4, 4);
		//graphics->DrawRectangle(&thinpen, (INT)end.x - 2, (INT)end.y - 2, 4, 4);
	}
	
}

void Coordinate::drawRect(Graphics* graphics, Color color, float x, float y, float width, float height)
{
	Pen thinpen(color, 2);
	XMFLOAT2 point = toCoordinateSpace(XMFLOAT2{ x, y });
	graphics->DrawRectangle(&thinpen, (INT)point.x - width * 0.5f, (INT)point.y - height * 0.5f, width, height);
}

void Coordinate::drawCircle(Graphics* graphics, Color color, float x, float y, float radius, int accuracy)
{
	float factor = 360.f / (float)accuracy;
	float timer = 0.f;
	for (int i = 0; i < accuracy; ++i)
	{
		float sx = (cosf((i * factor) * 3.141592f / 180.f)) * radius + x;
		float sy = (sinf((i * factor) * 3.141592f / 180.f)) * radius + y;

		float ex = (cosf(((i + 1) * factor) * 3.141592f / 180.f)) * radius + x;
		float ey = (sinf(((i + 1) * factor) * 3.141592f / 180.f)) * radius + y;

		drawLine(graphics, color, sx, sy, ex, ey,false);
	}
}

void Coordinate::draw(Graphics* graphics)
{
	

	Pen pen(_color, 2);
	Pen thinpen(_color, 1);

	PointF pt(10.f, 10.f);
	SolidBrush  solidBrush(Color(255, 0, 0, 0));

	
	graphics->DrawString(std::to_wstring(_ratioX).c_str(), -1,_font, PointF(10.f,10.f), &solidBrush);
	graphics->DrawString(std::to_wstring(_ratioY).c_str(), -1, _font, PointF(10.f, 20.f), &solidBrush);

	
	graphics->DrawLine(&pen, _centerX, -_windowY, _centerX, _windowY);
	graphics->DrawLine(&pen, -_windowX,_centerY, _windowX, _centerY);

	int x = _ratioX * 10;
	int y = _ratioY * 10;

	//x = x % 10;

	//x -= (_windowX / 2) % x;
	//y -= (_windowY / 2) % y;

	for (int i = _centerX % x; i < _windowX; i += x)
	{
		graphics->DrawLine(&thinpen,i,0,i,_windowY);
	}

	for (int i = _centerY % y; i < _windowY; i += y)
	{
		graphics->DrawLine(&thinpen, 0, i, _windowX, i);
	}
}
