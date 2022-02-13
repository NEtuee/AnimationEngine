#include "Triangulation.h"
#include "MathEx.h"

#include <set>

void Triangulation::initialize()
{
	_points.clear();
	_triangles.clear();
}

void Triangulation::addPoint(float x, float y)
{
	_points.push_back(XMFLOAT2(x, y));
}

void Triangulation::triangulation()
{
	int n = static_cast<int>(_points.size());
	_triangles.clear();

	_points.push_back(XMFLOAT2(-2e9, -2e9));
	_points.push_back(XMFLOAT2(2e9, -2e9));
	_points.push_back(XMFLOAT2(0, 2e9));
	_triangles.push_back(Triangle(n, n + 1, n + 2));

	for (int i = 0; i < n; i++) {
		std::set<Edge> polygon;
		std::vector<int> complete(_triangles.size(), 0);
		for (int j = 0; j < _triangles.size(); j++) 
		{
			if (complete[j]) 
				continue;
			Triangle cur = _triangles[j];
			if (isInCircum(cur, _points[i])) 
			{
				if (polygon.count(Edge(cur._a, cur._b)))
					polygon.erase(Edge(cur._a, cur._b)); // 만약 겹치는 edge라면 제거
				else
					polygon.insert(Edge(cur._a, cur._b)); // 안 겹치면 삽입
				if (polygon.count(Edge(cur._b, cur._c)))
					polygon.erase(Edge(cur._b, cur._c));
				else
					polygon.insert(Edge(cur._b, cur._c));
				if (polygon.count(Edge(cur._c, cur._a)))
					polygon.erase(Edge(cur._c, cur._a));
				else polygon.insert(Edge(cur._c, cur._a));

				std::swap(complete[j], complete[_triangles.size() - 1]); // bad _triangles 은 제거한다.
				std::swap(_triangles[j], _triangles[_triangles.size() - 1]);
				_triangles.pop_back();
				j--;
				continue;
			}
			complete[j] = true;
		}
		for (auto& cur : polygon) {
			XMVECTOR a = XMVectorSet(_points[cur._a].x, _points[cur._a].y, .0f, 0.f),
					 b = XMVectorSet(_points[cur._b].x, _points[cur._b].y, .0f, 0.f),
					 pointVec = XMVectorSet(_points[i].x, _points[i].y, 0.f, 0.f);
			
			if (XMVectorGetX(XMVector2Cross(b - a, pointVec - a)) == 0) 
				continue; // 일직선이므로 삼각형이 될 수 없다. 따라서 무시
			_triangles.push_back(Triangle(cur._a, cur._b, i));
		}
	}

	// SuperTriangle delete
	for (int i = 0; i < _triangles.size(); i++) {
		Triangle cur = _triangles[i];
		if (cur._a >= n || cur._b >= n || cur._c >= n) { // n ~ n+2 의 정점을 사용하는 삼각형은 모두 처분한다.
			std::swap(_triangles[i], _triangles[_triangles.size() - 1]);
			_triangles.pop_back();
			i--;
			continue;
		}
	}

	_points.pop_back();
	_points.pop_back();
	_points.pop_back();
}

bool Triangulation::isInCircum(Triangle cur, XMFLOAT2 point)
{
	XMVECTOR a = XMVectorSet(_points[cur._a].x, _points[cur._a].y, .0f,0.f), 
			 b = XMVectorSet(_points[cur._b].x, _points[cur._b].y, .0f, 0.f), 
			 c = XMVectorSet(_points[cur._c].x, _points[cur._c].y, .0f, 0.f);

	float ccw = XMVectorGetX(XMVector2Cross((b - a), (c - a)));

	float adx = _points[cur._a].x - point.x,
		ady = _points[cur._a].y - point.y,
		bdx = _points[cur._b].x - point.x,
		bdy = _points[cur._b].y - point.y,
		cdx = _points[cur._c].x - point.x,
		cdy = _points[cur._c].y - point.y,
		bdxcdy = bdx * cdy, cdxbdy = cdx * bdy,
		cdxady = cdx * ady, adxcdy = adx * cdy,
		adxbdy = adx * bdy, bdxady = bdx * ady,
		alift = adx * adx + ady * ady,
		blift = bdx * bdx + bdy * bdy,
		clift = cdx * cdx + cdy * cdy;
	float det = alift * (bdxcdy - cdxbdy)
		+ blift * (cdxady - adxcdy)
		+ clift * (adxbdy - bdxady);

	if (ccw > 0) return det >= 0;
	else return det <= 0;
}