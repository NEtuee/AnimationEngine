#include "Input.h"

Input* Input::getInstance()
{
	static Input input;
	return &input;
}

bool Input::isDown(unsigned int code)
{
	return _keyInput[code];
}

bool Input::isUp(unsigned int code)
{
	return _keyInput[code] == false;
}

void Input::updateMousePosition(float x, float y)
{
	_mouseX = x;
	_mouseY = y;
}

void Input::setKeyDown(unsigned int code)
{
	_keyInput[code] = true;
}

void Input::setKeyUp(unsigned int code)
{
	_keyInput[code] = false;
}

float Input::getMouseX()
{
	return _mouseX;
}

float Input::getMouseY()
{
	return _mouseY;
}
