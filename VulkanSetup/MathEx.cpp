#include "MathEx.h"
#include <math.h>

namespace MathEx
{
	float abs(float x)
	{
		return x < 0 ? -x : x;
	}

	float clamp01(float value)
	{
		return value < 0.f ? 0.f : (value > 1.f ? 1.f : value);
	}

	bool similar(float x, float y, float factor)
	{
		return abs(x - y) <= factor;
	}

	bool similarVec3(XMVECTOR x, XMVECTOR y, float factor)
	{
		return XMVectorGetX(XMVector3Length(x - y)) <= factor;
	}

	bool linear(XMVECTOR one, XMVECTOR two, float factor, bool checkZero)
	{
		if (checkZero && similarVec3(one, two, factor))
		{
			return true;
		}

		float angle = XMVectorGetX(XMVector3Dot(XMVector3Normalize(one), XMVector3Normalize(two)));
		
		return similar(angle, 1.f, factor);

	}

	float getSlope2D(XMFLOAT3 a, XMFLOAT3 b)
	{
		float x = b.x - a.x;
		float y = b.y - a.y;

		return y / x;
	}



	XMFLOAT3 mul(XMFLOAT3 target, float value)
	{
		return XMFLOAT3(target.x * value, target.y * value, target.z * value);
	}

	XMFLOAT3 sub(XMFLOAT3 target, XMFLOAT3 value)
	{
		return XMFLOAT3(target.x - value.x, target.y - value.y, target.z - value.z);
	}

	XMFLOAT3 add(XMFLOAT3 target, XMFLOAT3 value)
	{
		return XMFLOAT3(target.x + value.x, target.y + value.y, target.z + value.z);
	}

	XMFLOAT4 quaternionToAxisAngle(const XMVECTOR& quaternion)
	{
		XMFLOAT4 result;
		XMStoreFloat4(&result, quaternion);

		float angle = 2.f * acosf(result.w);
		float s = sqrtf(1.f - result.w * result.w);
		if (s > 0.001f)
		{
			result.x = result.x / s;
			result.y = result.y / s;
			result.z = result.z / s;
		}

		result.w = angle;

		return result;
	}

	XMFLOAT3 quaternionToEuler(const XMVECTOR& quaternion)
	{
		XMFLOAT3 result;

		//왜 전치함????
		XMMATRIX rotMatrix = XMMatrixTranspose(XMMatrixRotationQuaternion(quaternion));
		XMFLOAT4X4 realMatrix;
		XMStoreFloat4x4(&realMatrix, rotMatrix);

		result.x = asinf(-realMatrix._23);
		result.y = atan2f(realMatrix._13, realMatrix._33);
		result.z = atan2f(realMatrix._21, realMatrix._22);

		return result;
	}

	XMVECTOR toVector(const XMFLOAT3& float3)
	{
		return XMVectorSet(float3.x, float3.y, float3.z, 0.f);
	}

	XMVECTOR toVector(const XMFLOAT2& float2)
	{
		return XMVectorSet(float2.x, float2.y, 0.f, 0.f);
	}

	XMVECTOR LookAtQuaternion(XMVECTOR from, XMVECTOR to)
	{
		XMVECTOR forward = XMVector3Normalize(to - from);
		XMVECTOR dot = XMVector3Dot(XMVectorSet(0.f, 0.f, 1.f, 0.f), forward);
		float fdot = XMVectorGetX(dot);

		if (abs(fdot - (-1.0f)) < 0.000001f)
		{
			return XMVectorSet(0.f, 1.f, 0.f, 3.141592f);
		}
		if (abs(fdot - (1.0f)) < 0.000001f)
		{
			return XMQuaternionIdentity();
		}

		float rotAngle = (float)acos(fdot);
		XMVECTOR rotAxis = XMVector3Cross(XMVectorSet(0.f, 0.f, 1.f, 0.f), forward);
		rotAxis = XMVector3Normalize(rotAxis);

		return XMQuaternionRotationAxis(rotAxis, rotAngle);
	}

	XMVECTOR quaternionBarycentric(FXMVECTOR Q0, FXMVECTOR Q1, FXMVECTOR Q2, float w1, float w2)
	{
		// Note if you choose one of the three weights to be zero, you get a blend of two
		//  quaternions.  This does not give you slerp of those quaternions.
		float w0 = 1.0f - w1 - w2;
		XMVECTOR Result = XMVector4Normalize(
			XMVectorScale(Q0, w1) +
			XMVectorScale(Q1, w2) +
			XMVectorScale(Q2, w0));
		return Result;
	}

	XMFLOAT3 radianToDegree(const XMFLOAT3& radian)
	{
		XMFLOAT3 degree = XMFLOAT3{ XMConvertToDegrees(radian.x),XMConvertToDegrees(radian.y) ,XMConvertToDegrees(radian.z) };
		//degree.x += degree.x < 0.f ? 180.f : 0.f;
		return degree;
	}

	XMFLOAT3 degreeToRadian(const XMFLOAT3& degree)
	{
		return XMFLOAT3{ XMConvertToRadians(degree.x),XMConvertToRadians(degree.y) ,XMConvertToRadians(degree.z) };
	}

	bool isInTriangle(XMVECTOR target, XMVECTOR a, XMVECTOR b, XMVECTOR c, float& t, float& s, float& ots)
	{
		getBarycentricWeight(target, a, b, c, t, s, ots);

		return t >= 0 && t <= 1 && s >= 0 && s <= 1 && ots >= 0 && ots <= 1;
	}

	void getBarycentricWeight(XMVECTOR target, XMVECTOR a, XMVECTOR b, XMVECTOR c, float& t, float& s, float& ots)
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

	XMVECTOR getPerpendicularPointOnLineSegment(XMVECTOR lineA, XMVECTOR lineB, XMVECTOR point)
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

	bool findNearestPointOnTriangle(XMVECTOR target, XMVECTOR a, XMVECTOR b, XMVECTOR c, XMVECTOR& result, float& nearDistance, float& t, float& s, float& ots, int& edge)
	{
		if (isInTriangle(target, a, b, c, t, s, ots))
		{
			return false;
		}


		result = target;
		nearDistance = INFINITE;
		if (s < 0)
		{
			result = getPerpendicularPointOnLineSegment(a, b, target);

			nearDistance = XMVectorGetX(XMVector3Length(result - target));
			edge = 0;
		}

		if (t < 0)
		{
			XMVECTOR point = getPerpendicularPointOnLineSegment(b, c, target);

			float distance = XMVectorGetX(XMVector3Length(point - target));

			if (nearDistance > distance)
			{
				nearDistance = distance;
				result = point;
				edge = 1;
			}
		}

		if (ots < 0)
		{
			XMVECTOR point = getPerpendicularPointOnLineSegment(c, a, target);

			float distance = XMVectorGetX(XMVector3Length(point - target));

			if (nearDistance > distance)
			{
				nearDistance = distance;
				result = point;
				edge = 2;
			}
		}

		getBarycentricWeight(result, a, b, c, t, s, ots);

		return true;
	}

}