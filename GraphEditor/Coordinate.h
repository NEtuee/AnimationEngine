#pragma once
#include <ObjIdl.h>
#include <gdiplus.h>
#include "../VulkanSetup/DirectXDefine.h"

using namespace Gdiplus;

class Coordinate
{
public:
	void createCoordinate(Color color, int centerX, int centerY, int windowX, int windowY);
	void setRatio(int x, int y);
	void addRatioX(int value);
	void addRatioY(int value);

	void addCenterX(int value);
	void addCenterY(int value);

	XMFLOAT2 toCoordinateSpace(XMFLOAT2 world);
	XMFLOAT2 toWorldSpace(XMFLOAT2 coord);

	void drawString(Graphics* graphics, Color color, const WCHAR* str, float x, float y);
	void drawLine(Graphics* graphics,Color color, float sx, float sy, float ex, float ey, bool rect = true);
	void drawRect(Graphics* graphics, Color color, float x, float y, float width, float height);
	void drawCircle(Graphics* graphics, Color color, float x, float y, float radius, int accuracy);
	void draw(Graphics* graphics);
private:
	int _windowX = 0;
	int _windowY = 0;

	int _centerX = 0;
	int _centerY = 0;
	int _ratioX = 0;
	int _ratioY = 0;
	Color _color;
	Font* _font = nullptr;
};

