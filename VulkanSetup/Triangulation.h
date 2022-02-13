#pragma once
#include "DirectXDefine.h"
#include <vector>

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

class Triangulation
{
public:
	void		initialize();

	void		addPoint(float x, float y);
	void		triangulation();

	bool		isInCircum(Triangle cur, XMFLOAT2 point);

	const std::vector<XMFLOAT2>&	getPoints()			{ return _points; }
	const std::vector<Triangle>&	getTriangles()		{ return _triangles; }
public:
	std::vector<XMFLOAT2>			_points;
	std::vector<Triangle>			_triangles;
};

