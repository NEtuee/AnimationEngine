#include "HalfQuaternion.h"
#include "Serialize.h"
#include <Math.h>
//00 == x, 01 = y, 10 = z, 11 = w
char HalfQuaternion::findBiggest(HalfFloat** items, int size)
{
	char biggest = 0;
	for (int i = 0; i < size; ++i)
	{
		if (items[i]->getFloat() > items[biggest]->getFloat())
		{
			biggest = i;
			//biggest |= (items[(biggest & 0b011)]->getReal() >> 13);
		}
	}

	biggest |= ((items[biggest]->getReal() >> 13) & 0b100);
	if (biggest > 3)
		int j = 0;
	return biggest;
}

void HalfQuaternion::serialize(Serialization* serialize, std::ostream* stream)
{
	unsigned long long int bytes = 0;

	HalfFloat* items[4] = { &_x, &_y, &_z, &_w };
	char biggestFlag = findBiggest(items,4);
	bytes = biggestFlag << 5;
	bytes = bytes << 40;
	
	int position = 30;
	for (int i = 0; i < 4; ++i)
	{
		if ((biggestFlag & 0b011) == i)
			continue;

		unsigned long long int hf = 0;
		*items[i] += 1.f;
		hf |= items[i]->getReal() & 0x7fff;
		bytes |= hf << position;

		position -= 15;
	}

	serialize->write(stream, &bytes,6);
}

void HalfQuaternion::deserialize(Serialization* serialize, std::istream* stream)
{
	unsigned long long int bytes = 0;
	serialize->read(stream, &bytes, 6);

	char biggestFlag = (bytes >> 45) & 0b111;
	float sign = (biggestFlag & 0b100) == 0 ? 1.f : -1.f;

	HalfFloat* items[4] = { &_x, &_y, &_z, &_w };
	
	float square = 0.f;
	int position = 30;
	int biggestTarget = 0;
	for (int i = 0; i < 4; ++i)
	{
		if ((biggestFlag & 0b011) == i)
		{
			biggestTarget = i;
			continue;
		}
		unsigned short real;
		real = (bytes >> position) & 0x7fff;
		items[i]->setReal(real);
		*items[i] += -1.f;

		square += items[i]->getFloat() * items[i]->getFloat();

		position -= 15;
	}

	items[biggestTarget]->setFloat(sqrtf(1.f - square) * sign);
	
}
