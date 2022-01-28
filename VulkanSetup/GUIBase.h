#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>

class GUIBase
{
public:
	virtual void initialize() = 0;
	virtual void frame() = 0;
	virtual void render() = 0;
};

