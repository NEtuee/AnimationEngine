#pragma once
#include "HalfFloat.h"
#include <iostream>

class Serialization;
class HalfQuaternion
{
public:
	HalfFloat _x;
	HalfFloat _y;
	HalfFloat _z;
	HalfFloat _w;

	HalfQuaternion() {}
	HalfQuaternion(float x, float y, float z, float w)
	{
		_x.setFloat(x);
		_y.setFloat(y);
		_z.setFloat(z);
		_w.setFloat(w);
	}

	void StoreQuaternion(XMVECTOR vec)
	{
		XMFLOAT4 f4;
		XMStoreFloat4(&f4, vec);

		*this = f4;
	}

	char					findBiggest(HalfFloat** items, int size);

	void					serialize(Serialization* serialize, std::ostream* stream);
	void					deserialize(Serialization* serialize, std::istream* stream);

	HalfQuaternion			operator=(XMFLOAT4& f4)				{ _x = f4.x; _y = f4.y; _z = f4.z; _w = f4.w; return *this; }
	HalfQuaternion			operator=(HalfQuaternion f4)		{ _x = f4._x; _y = f4._y; _z = f4._z; _w = f4._w; return *this; }
	bool					operator==(HalfQuaternion& value)	{ return _x == value._x && _y == value._y && _z == value._z && _w == value._w; }

	static HalfQuaternion	zero()								{ return HalfQuaternion{ 0.f,0.f,0.f,0.f }; }
	static HalfQuaternion	one()								{ return HalfQuaternion{ 1.f,1.f,1.f,1.f }; }
	static HalfQuaternion	quaternionIdentity()				{ return HalfQuaternion{ 0.f,0.f,0.f,1.f }; }
};

