#pragma once
#include "DirectXDefine.h"
#include <iostream>
#include <stdint.h>

class Serialization;
class CompressedQuaternion
{
public:
	CompressedQuaternion() {}
	CompressedQuaternion(float x, float y, float z, float w)	{ setQuaternion(x, y, z, w); }

	XMFLOAT4						getQuaternion() const;
	void							setQuaternion(float x, float y, float z, float w);

	void							serialize(Serialization* serialize, std::ostream* stream);
	void							deserialize(Serialization* serialize, std::istream* stream);

	void							StoreQuaternion(XMVECTOR vector);

	static CompressedQuaternion		zero()					{ return CompressedQuaternion{ 0.f,0.f,0.f,0.f }; }
	static CompressedQuaternion		one()					{ return CompressedQuaternion{ 1.f,1.f,1.f,1.f }; }
	static CompressedQuaternion		quaternionIdentity()	{ return CompressedQuaternion{ 0.f,0.f,0.f,1.f }; }

private:
	float							toFloat(unsigned short bit) const;
	unsigned short					toHalf(float value);
private:
	unsigned short					_values[4] = { 0 };
};

