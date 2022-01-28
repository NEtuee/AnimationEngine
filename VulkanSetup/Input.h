#pragma once
#include <unordered_map>

class Input
{
public:
	static Input*	getInstance();

	bool			isDown(unsigned int code);
	bool			isUp(unsigned int code);

	void			updateMousePosition(float x, float y);

	void			setKeyDown(unsigned int code);
	void			setKeyUp(unsigned int code);

	float			getMouseX();
	float			getMouseY();
private:
	std::unordered_map<unsigned int, bool> _keyInput;
	float _mouseX;
	float _mouseY;
};

