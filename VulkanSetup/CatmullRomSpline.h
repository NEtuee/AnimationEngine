#pragma once
#include <vector>
#include "../VulkanSetup/DirectXDefine.h"

class CatmullRomSpline
{
public:
	XMVECTOR	getSpline(size_t position, float factor);
	int			getSize();

	void		addPoint(XMVECTOR point);
	void		insertPoint(XMVECTOR point, size_t index);
	void		clearPoint();

	XMVECTOR	getPoint(size_t index);

	XMVECTOR	hermiteSpline(XMVECTOR one, XMVECTOR two, XMVECTOR d1, XMVECTOR d2, float t);
	float		splineLength(XMVECTOR const& start, XMVECTOR const& start_tangent,
					XMVECTOR const& end, XMVECTOR const& end_tangent);
private:
	XMVECTOR	getSpeed(size_t index);
private:
	std::vector<XMVECTOR> _points;
	float		_minimum;
	float		_maximum;
};

