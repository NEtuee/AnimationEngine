#pragma once
#include <math.h>

struct Vector2D 
{
	double x;
	double y;
	Vector2D() { x = y = 0; }
	Vector2D(double x, double y) 
	{
		this->x = x;
		this->y = y;
	}
	const double dist() const 
	{
		return sqrt(x * x + y * y);
	}
	const double cross(const Vector2D& a) const
	{
		return x * a.y - y * a.x;
	}
	const Vector2D operator+ (const Vector2D& a) const
	{
		return Vector2D(x + a.x, y + a.y);
	}
	const Vector2D operator- (const Vector2D& a) const
	{
		return Vector2D(x - a.x, y - a.y);
	}
	const Vector2D operator* (const double& a) const
	{
		return Vector2D(a * x, a * y);
	}
};

struct Edge 
{
	int _a;
	int _b;
	Edge() :_a(0), _b(0) {}
	Edge(int a, int b)
	{
		if (a < b)
		{
			_a = a;
			_b = b;
		}
		else
		{
			_a = b;
			_b = a;
		}
	}

	const bool operator==(const Edge& x)const
	{
		return _a == x._a && _b == x._b;
	}
	
	const bool operator<(const Edge& x)const
	{
		if (_a == x._a)
			return _b < x._b;

		return _a < x._a;
	}
};

struct Triangle
{
	int _a;
	int _b;
	int _c;
	Triangle() :_a(0), _b(0), _c(0) {}
	Triangle(int a, int b, int c)
	{
		_a = a;
		_b = b;
		_c = c;
	}
};