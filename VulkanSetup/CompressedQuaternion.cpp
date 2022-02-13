#include "CompressedQuaternion.h"
#include <Math.h>
#include "MathEx.h"
#include "Quantization.h"
#include "Serialize.h"
#include <iostream>

XMFLOAT4 CompressedQuaternion::getQuaternion() const
{
	return XMFLOAT4{ toFloat(_values[0]),toFloat(_values[1]),toFloat(_values[2]),toFloat(_values[3]) };
}

void CompressedQuaternion::setQuaternion(float x, float y, float z, float w)
{
	_values[0] = toHalf(x);
	_values[1] = toHalf(y);
	_values[2] = toHalf(z);
	_values[3] = toHalf(w);

	for (int i = 0; i < 4; ++i)
	{
		if (toFloat(_values[i]) > 1.f)
			assert(false);
	}
}

void CompressedQuaternion::serialize(Serialization* serialize, std::ostream* stream)
{
	int biggest = 0;
	for (int i = 0; i < 4; ++i)
	{

		if (MathEx::abs(toFloat(_values[biggest])) < MathEx::abs(toFloat(_values[i])))
		{
			biggest = i;
		}
	}

	unsigned long long int bits = 0;
	bits |= toFloat(_values[biggest]) > 0 ? 0 : 0b1;
	bits <<= 2;
	bits |= biggest;

	for (int i = 0; i < 4; ++i)
	{
		if (i == biggest)
			continue;

		bits <<= 15;
		bits |= _values[i];
	}

	serialize->write(stream, &bits, 6);
}

void CompressedQuaternion::deserialize(Serialization* serialize, std::istream* stream)
{
	unsigned long long int bits = 0;
	serialize->read(stream, &bits, 6);

	bool sign = (bits >> 47) == 1;
	int biggest = (bits >> 45) & 0b011;
	int position = 30;

	float total = 0.f;

	for (int i = 0; i < 4; ++i)
	{
		if (i == biggest)
			continue;

		_values[i] = (bits >> position) & 0x7fff;
		float decompressed = toFloat(_values[i]);
		total += decompressed * decompressed;

		position -= 15;
	}

	_values[biggest] = toHalf(sqrtf(1.f - total) * (sign ? -1.f : 1.f));

	for (int i = 0; i < 4; ++i)
	{
		if (toFloat(_values[i]) > 1.f)
		{
			int s = 0;
		}

	}
}

void CompressedQuaternion::StoreQuaternion(XMVECTOR quaternion)
{
	XMFLOAT4 stored;
	XMStoreFloat4(&stored, quaternion);

	setQuaternion(stored.x, stored.y, stored.z, stored.w);
}


float CompressedQuaternion::toFloat(unsigned short bit) const
{
	return Quantization::decompressFloatMinMax(bit, -1.f, 1.f, 15);
}

unsigned short CompressedQuaternion::toHalf(float value)
{
	return Quantization::compressFloatMinMax(value, -1.f, 1.f, 15);
}
