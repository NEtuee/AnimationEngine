#pragma once
#include <math.h>
#include <vector>
#include "Defines.h"
#include "../VulkanSetup/DirectXDefine.h"

struct myPoint
{
	double x;
	double y;
};

class Math
{
public:
	static bool findNearestPointOnTriangle(XMVECTOR target, XMVECTOR a, XMVECTOR b, XMVECTOR c,XMVECTOR& result, 
												float& nearDistance,float& t,float& s,float& ots)
	{
		if (isInTriangle(target, a, b, c, t, s, ots))
		{
			return false;
		}


		result = target;
		nearDistance = INFINITE;
		if (s < 0)
		{
			result = Math::perpendicularPointOnLineSegment(a, b, target);

			nearDistance = XMVectorGetX(XMVector3Length(result - target));
		}

		if (t < 0)
		{
			XMVECTOR point = Math::perpendicularPointOnLineSegment(b, c, target);

			float distance = XMVectorGetX(XMVector3Length(point - target));

			if (nearDistance > distance)
			{
				nearDistance = distance;
				result = point;
			}
		}

		if (ots < 0)
		{
			XMVECTOR point = Math::perpendicularPointOnLineSegment(c,a,target);

			float distance = XMVectorGetX(XMVector3Length(point - target));

			if (nearDistance > distance)
			{
				nearDistance = distance;
				result = point;
			}
		}

		return true;
	}
	static XMVECTOR perpendicularPointOnLineSegment(XMVECTOR lineA,XMVECTOR lineB, XMVECTOR point)
	{
		XMVECTOR pa = point - lineA;
		XMVECTOR ab = lineB - lineA;

		XMVECTOR len = XMVector3LengthSq(ab);
		XMVECTOR dot = XMVector3Dot(ab, pa);
		XMVECTOR distance = dot / len;

		if (XMVectorGetX(distance) < 0)
		{
			return lineA;
		}
		else if (XMVectorGetX(distance) > 1)
		{
			return lineB;
		}
		else
		{
			return lineA + ab * XMVectorGetX(distance);
		}

	}

	static bool isInTriangle(XMVECTOR target, XMVECTOR a, XMVECTOR b, XMVECTOR c, float& t, float& s, float& ots)
	{
		getBarycentricCoord(target, a, b, c, t, s, ots);

		return t >= 0 && t <= 1 && s >= 0 && s <= 1 && ots >= 0 && ots <= 1;
	}

	static void getBarycentricCoord(XMVECTOR target, XMVECTOR a, XMVECTOR b, XMVECTOR c, float& t, float& s, float& ots)
	{
		XMVECTOR u = c - b;
		XMVECTOR v = a - b;

		XMVECTOR w = target - b;

		float uu = XMVectorGetX(XMVector2Dot(u, u));
		float vv = XMVectorGetX(XMVector2Dot(v, v));
		float uv = XMVectorGetX(XMVector2Dot(u, v));
		float wu = XMVectorGetX(XMVector2Dot(w, u));
		float wv = XMVectorGetX(XMVector2Dot(w, v));

		float denominator = uv * uv - uu * vv;
		t = (wu * uv - wv * uu) / denominator;
		s = (wv * uv - wu * vv) / denominator;
		ots = 1.f - s - t;

	}
	static bool isInCircum(Triangle cur, int i, std::vector<Vector2D>& point) { // 외접원안에 점이 들어오는지 확인
		double ccw = (point[cur._b] - point[cur._a]).cross(point[cur._c] - point[cur._a]);

		double adx = point[cur._a].x - point[i].x, 
			ady = point[cur._a].y - point[i].y,
			bdx = point[cur._b].x - point[i].x, 
			bdy = point[cur._b].y - point[i].y,
			cdx = point[cur._c].x - point[i].x, 
			cdy = point[cur._c].y - point[i].y,
			bdxcdy = bdx * cdy, cdxbdy = cdx * bdy,
			cdxady = cdx * ady, adxcdy = adx * cdy,
			adxbdy = adx * bdy, bdxady = bdx * ady,
			alift = adx * adx + ady * ady,
			blift = bdx * bdx + bdy * bdy,
			clift = cdx * cdx + cdy * cdy;
		double det = alift * (bdxcdy - cdxbdy)
			+ blift * (cdxady - adxcdy)
			+ clift * (adxbdy - bdxady);

		if (ccw > 0) return det >= 0;
		else return det <= 0;
	}

	static void lineFromPoints(myPoint p, myPoint q, double& a, double& b, double& c)
	{
		a = q.y - p.y;
		b = p.x - q.x;
		c = a * (p.x) + b * (p.y);
	}

	static void perpendicularBisectorFromLine(myPoint p, myPoint q, double& a, double& b, double& c)
	{
		myPoint mid;
		mid.x = (p.x + q.x) * 0.5;
		mid.y = (p.y + q.y) * 0.5;

		c = -b * (mid.x) + a * (mid.y);
		double temp = a;
		a = -b;
		b = temp;
	}

	static myPoint lineIntersection(double a1, double b1, double c1, double a2, double b2, double c2)
	{
		double determinant = a1 * b2 - a2 * b1;
		if (determinant == 0)
		{
			return myPoint{ 0,0 };
		}
		else
		{
			double x = (b2 * c1 - b1 * c2) / determinant;
			double y = (a1 * c2 - a2 * c1) / determinant;
			return myPoint{ x,y };
		}
	}

	static myPoint findCircumCenter(myPoint p, myPoint q, myPoint r)
	{
		double a, b, c;
		lineFromPoints(p, q, a, b, c);
		double e, f, g;
		lineFromPoints(q, r, e, f, g);

		perpendicularBisectorFromLine(p, q, a, b, c);
		perpendicularBisectorFromLine(q, r, e, f, g);

		myPoint circumcenter = lineIntersection(a, b, c, e, f, g);
		return circumcenter;
	}
	static double findCircumRadius(myPoint a, myPoint b, myPoint c)
	{
		double ax = c.x - b.x;
		double ay = c.y - b.y;
		double bx = a.x - c.x;
		double by = a.y - c.y;
		double cross = ax * by - ay * bx;
		if (cross != 0)
		{
			double as = sqrt(ax * ax + ay * ay);
			double bs = sqrt(bx * bx + by * by);

			double cx = b.x - a.x;
			double cy = b.y - a.y;

			double cs = sqrt(cx * cx + cy * cy);

			return (0.5 * as * bs * cs / fabs(cross));
		}
		else
			return -1;
	}
	//static double distance(myPoint a, myPoint b)
	//{
	//	double x = abs(a.x - b.x);
	//	double y = abs(a.y - b.y);

	//	return sqrt(x * x + y * y);
	//}
};
