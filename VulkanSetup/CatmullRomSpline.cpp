#include "CatmullRomSpline.h"
#include <assert.h>

XMVECTOR CatmullRomSpline::getSpline(size_t position, float factor)
{
	if (_points.size() < 3)
	{
		return XMVectorLerp(_points[0], _points[1],factor);
	}

	XMVECTOR dir1 = getSpeed(position);
	XMVECTOR dir2 = getSpeed(position + 1);

	return hermiteSpline(_points[position], _points[position + 1], dir1, dir2, factor);
}

int CatmullRomSpline::getSize()
{
	return static_cast<int>(_points.size());
}

void CatmullRomSpline::addPoint(XMVECTOR point)
{
	_points.push_back(point);
}

void CatmullRomSpline::insertPoint(XMVECTOR point, size_t index)
{
	_points.insert(_points.begin() + index, point);
}

void CatmullRomSpline::clearPoint()
{
	_points.clear();
}

XMVECTOR CatmullRomSpline::getPoint(size_t index)
{
	return _points[index];
}

XMVECTOR CatmullRomSpline::hermiteSpline(XMVECTOR one, XMVECTOR two, XMVECTOR d1, XMVECTOR d2, float t)
{
	float h0 = 2.f * (t * t * t) - 3.f * (t * t) + 1;
	float h1 = -2.f * (t * t * t) + 3.f * (t * t);
	float h2 = (t * t * t) - 2.f * (t * t) + t;
	float h3 = (t * t * t) - (t * t);

	XMVECTOR point;
	point = (one * h0) + (two * h1) + (d1 * h2) + (d2 * h3);
	return point;
}

//https://medium.com/@all2one/how-to-compute-the-length-of-a-spline-e44f5f04c40
float CatmullRomSpline::splineLength(XMVECTOR const& start, XMVECTOR const& start_tangent, XMVECTOR const& end, XMVECTOR const& end_tangent)
{
	// Cubic Hermite spline derivative coeffcients
	XMVECTOR const c0 = start_tangent;
	XMVECTOR const c1 = 6.f * (end - start) - 4.f * start_tangent - 2.f * end_tangent;
	XMVECTOR const c2 = 6.f * (start - end) + 3.f * (start_tangent + end_tangent);
	auto const evaluate_derivative = [c0, c1, c2](float t) -> XMVECTOR
	{
		return c0 + t * (c1 + t * c2);
	};
	struct GaussLengendreCoefficient
	{
		float abscissa; // xi
		float weight;   // wi
	};
	static constexpr GaussLengendreCoefficient gauss_lengendre_coefficients[] =
	{
		   { 0.0f, 0.5688889f },
		   { -0.5384693f, 0.47862867f },
		   { 0.5384693f, 0.47862867f },
		   { -0.90617985f, 0.23692688f },
		   { 0.90617985f, 0.23692688f }
	};
	float length = 0.f;
	for (auto coefficient : gauss_lengendre_coefficients)
	{
		float const t = 0.5f * (1.f + coefficient.abscissa); // This and the final (0.5 *) below are needed for a change of interval to [0, 1] from [-1, 1]
		length += XMVectorGetX(XMVector3Length(evaluate_derivative(t))) * coefficient.weight;
	}
	return 0.5f * length;
}

XMVECTOR CatmullRomSpline::getSpeed(size_t index)
{
	if (index == 0)
		return _points[index + 2] - _points[index];
	else if (index >= _points.size() - 1)
		return XMVectorZero();
	else
		return (_points[index + 1] - _points[index - 1]) * 0.5f;
}
