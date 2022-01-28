#include "Triangulation.h"
#include "Math.h"
#include <set>

void Triangulation::createTriangulation()
{
	_points.clear();
	_triangles.clear();
}

void Triangulation::addPoint(float x, float y)
{
	_points.push_back(Vector2D(x, y));
}

void Triangulation::triangulation()
{
	int n = _points.size();
	_triangles.clear();

	_points.push_back(Vector2D(-2e9, -2e9));
	_points.push_back(Vector2D(2e9, -2e9));
	_points.push_back(Vector2D(0, 2e9));
	_triangles.push_back(Triangle(n, n + 1, n + 2));

	for (int i = 0; i < n; i++) {
		std::set<Edge> polygon;
		std::vector<int> complete(_triangles.size(), 0);
		for (int j = 0; j < _triangles.size(); j++) {
			if (complete[j]) continue;
			Triangle cur = _triangles[j];
			if (Math::isInCircum(cur, i, _points)) {
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
			if ((_points[cur._b] - _points[cur._a]).cross(_points[i] - _points[cur._a]) == 0) continue; // 일직선이므로 삼각형이 될 수 없다. 따라서 무시
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
