#pragma once
#include "DirectXDefine.h"

namespace MathEx

{
	template <typename T>
	static void swap(T& one, T& two)
	{
		T save = one;
		one = two;
		two = save;
	}

	float			abs(float x);
	int				absi(int x);
	float			clamp01(float value);
	bool			similar(float x, float y, float factor);
	bool			linear(XMVECTOR one, XMVECTOR two, float factor, bool checkZero = true);
	float			getSlope2D(XMFLOAT3 a, XMFLOAT3 b);
	float			lerp(float x, float y, float w);

	XMFLOAT3		mul(XMFLOAT3 target, float value);
	XMFLOAT3		sub(XMFLOAT3 target, XMFLOAT3 value);
	XMFLOAT3		add(XMFLOAT3 target, XMFLOAT3 value);

	XMFLOAT2		mul(XMFLOAT2 target, float value);
	XMFLOAT2		sub(XMFLOAT2 target, XMFLOAT2 value);
	XMFLOAT2		add(XMFLOAT2 target, XMFLOAT2 value);

	XMFLOAT4		quaternionToAxisAngle(const XMVECTOR& quaternion);
	XMFLOAT3		quaternionToEuler(const XMVECTOR& quaternion);
	XMVECTOR		toVector(const XMFLOAT3& float3);
	XMVECTOR		toVector(const XMFLOAT2& float2);
	XMVECTOR		lookAtQuaternion(XMVECTOR from, XMVECTOR to);
	XMVECTOR		fromToQuaternion(XMVECTOR from, XMVECTOR to, float angleScale = 1.f);

	XMFLOAT3		radianToDegree(const XMFLOAT3& radian);
	XMFLOAT3		degreeToRadian(const XMFLOAT3& degree);

	bool			isInTriangle(XMVECTOR target, XMVECTOR a, XMVECTOR b, XMVECTOR c, float& t, float& s, float& ots);
	void			getBarycentricWeight(XMVECTOR target, XMVECTOR a, XMVECTOR b, XMVECTOR c, float& t, float& s, float& ots);
	XMVECTOR		getPerpendicularPointOnLineSegment(XMVECTOR lineA, XMVECTOR lineB, XMVECTOR point);
	bool			findNearestPointOnTriangle(XMVECTOR target, XMVECTOR a, XMVECTOR b, XMVECTOR c, XMVECTOR& result,
					float& nearDistance, float& t, float& s, float& ots, int& edge);
	

}