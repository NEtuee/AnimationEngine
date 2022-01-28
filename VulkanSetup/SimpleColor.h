#pragma once
struct SimpleColor 
{
	float _r;
	float _g;
	float _b;
	float _a;
	SimpleColor() : _r(0.f), _g(0.f), _b(0.f), _a(0.f) {}
	SimpleColor(float r, float g, float b, float a) : _r(r), _g(g), _b(b), _a(a) {}
};