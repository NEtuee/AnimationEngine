#pragma once
#include "Defines.h"
#include <vector>

class Triangulation
{
public:
	void createTriangulation();

	void addPoint(float x,float y);
	void triangulation();

	const std::vector<Vector2D>& getPoints() { return _points; }
	const std::vector<Triangle>& getTriangles() { return _triangles; }
public:
	std::vector<Vector2D> _points;
	std::vector<Triangle> _triangles;
};

