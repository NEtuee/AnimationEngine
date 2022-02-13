#pragma once
#include "DirectXDefine.h"

class HalfFloat
{
public:
	void			setFloat(float input);
	float			getFloat() const;

	void			setReal(unsigned short real);
	unsigned short	getReal();

					HalfFloat() :_half(0) {}

	HalfFloat		operator+=(const float& value)		{ setFloat(getFloat() + value); return *this; }
	HalfFloat		operator=(unsigned short& value)	{ _half = value; return *this; }
	HalfFloat		operator=(HalfFloat& value)			{ _half = value._half; return *this; }
	HalfFloat		operator=(float& value)				{ setFloat(value); return *this; }
	bool			operator==(float& value)			{ return getFloat() == value; }
	bool			operator==(HalfFloat& value)		{ return _half == value._half; }

	operator		float() const						{ return getFloat(); }
private:
	uint16_t		floatToHalf(unsigned int i);
	uint32_t		halfToFloat() const;
private:
	unsigned short	_half;
};



struct HalfFloat3
{
	HalfFloat	_x;
	HalfFloat	_y;
	HalfFloat	_z;

	HalfFloat3() {}
	HalfFloat3(float x, float y, float z)
	{
		_x.setFloat(x);
		_y.setFloat(y);
		_z.setFloat(z);
	}

	void StoreVector(XMVECTOR vec)
	{
		XMFLOAT3 f3;
		XMStoreFloat3(&f3, vec);

		*this = f3;
	}

	bool		operator==(HalfFloat3& value)	{ return _x == value._x && _y == value._y && _z == value._z; }
	HalfFloat3	operator=(XMFLOAT3& f3)			{ _x = f3.x; _y = f3.y; _z = f3.z; return *this; }
	HalfFloat3	operator=(HalfFloat3 f3)		{ _x = f3._x; _y = f3._y; _z = f3._z; return *this; }

	static		HalfFloat3 zero()				{ return HalfFloat3{ 0.f,0.f,0.f }; }
	static		HalfFloat3 one()				{ return HalfFloat3{ 1.f,1.f,1.f }; }

};

struct HalfFloat4
{
	HalfFloat		_x;
	HalfFloat		_y;
	HalfFloat		_z;
	HalfFloat		_w;

	HalfFloat4() {}
	HalfFloat4(float x, float y, float z, float w)
	{
		_x.setFloat(x);
		_y.setFloat(y);
		_z.setFloat(z);
		_w.setFloat(w);
	}

	void StoreVector(XMVECTOR vec)
	{
		XMFLOAT4 f4;
		XMStoreFloat4(&f4, vec);

		*this = f4;
	}

	HalfFloat4			operator=(XMFLOAT4& f4)			{ _x = f4.x; _y = f4.y; _z = f4.z; _w = f4.w; return *this; }
	HalfFloat4			operator=(HalfFloat4 f4)		{ _x = f4._x; _y = f4._y; _z = f4._z; _w = f4._w; return *this; }
	bool				operator==(HalfFloat4& value)	{ return _x == value._x && _y == value._y && _z == value._z && _w == value._w; }

	static HalfFloat4	zero()							{ return HalfFloat4{ 0.f,0.f,0.f,0.f }; }
	static HalfFloat4	one()							{ return HalfFloat4{ 1.f,1.f,1.f,1.f }; }
	static HalfFloat4	quaternionIdentity()			{ return HalfFloat4{ 0.f,0.f,0.f,1.f }; }
};
