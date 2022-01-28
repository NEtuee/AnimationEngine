﻿#pragma once
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
	float			clamp01(float value);
	bool			similar(float x, float y, float factor);
	bool			linear(XMVECTOR one, XMVECTOR two, float factor, bool checkZero = true);
	float			getSlope2D(XMFLOAT3 a, XMFLOAT3 b);

	XMFLOAT3		mul(XMFLOAT3 target, float value);
	XMFLOAT3		sub(XMFLOAT3 target, XMFLOAT3 value);
	XMFLOAT3		add(XMFLOAT3 target, XMFLOAT3 value);
	XMFLOAT4		quaternionToAxisAngle(const XMVECTOR& quaternion);
	XMFLOAT3		quaternionToEuler(const XMVECTOR& quaternion);
	XMVECTOR		toVector(const XMFLOAT3& float3);
	XMVECTOR		toVector(const XMFLOAT2& float2);
	XMVECTOR		LookAtQuaternion(XMVECTOR from, XMVECTOR to);
	XMVECTOR		quaternionBarycentric(FXMVECTOR Q0, FXMVECTOR Q1, FXMVECTOR Q2, float w1, float w2);

	XMFLOAT3		radianToDegree(const XMFLOAT3& radian);
	XMFLOAT3		degreeToRadian(const XMFLOAT3& degree);

	bool			isInTriangle(XMVECTOR target, XMVECTOR a, XMVECTOR b, XMVECTOR c, float& t, float& s, float& ots);
	void			getBarycentricWeight(XMVECTOR target, XMVECTOR a, XMVECTOR b, XMVECTOR c, float& t, float& s, float& ots);
	XMVECTOR		getPerpendicularPointOnLineSegment(XMVECTOR lineA, XMVECTOR lineB, XMVECTOR point);
	bool			findNearestPointOnTriangle(XMVECTOR target, XMVECTOR a, XMVECTOR b, XMVECTOR c, XMVECTOR& result,
					float& nearDistance, float& t, float& s, float& ots, int& edge);
	

}