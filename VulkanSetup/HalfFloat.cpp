#include "HalfFloat.h"

void HalfFloat::setFloat(float input)
{
	union
	{
		float f;
		unsigned int i;
	} value = { input };
	
	_half = floatToHalf(value.i);
}

float HalfFloat::getFloat() const
{
	union
	{
		float f;
		unsigned int i;
	} value;

	value.i = halfToFloat();

	return value.f;
}

void HalfFloat::setReal(unsigned short real)
{
	_half = real;
}

unsigned short HalfFloat::getReal()
{
	return _half;
}

uint16_t HalfFloat::floatToHalf(unsigned int i)
{
	int s = (i >> 16) & 0x00008000;                  
	int e = ((i >> 23) & 0x000000ff) - (127 - 15);    
	int f = i & 0x007fffff;                

	if (e <= 0) 
	{
		if (e < -10) 
		{
			if (s)                                             
				return 0x8000;
			else
				return 0;
		}
		f = (f | 0x00800000) >> (1 - e);
		return s | (f >> 13);
	}
	else if (e == 0xff - (127 - 15)) 
	{
		if (f == 0)                                           
			return s | 0x7c00;
		else 
		{                                                  
			f >>= 13;
			return s | 0x7c00 | f | (f == 0);
		}
	}
	else 
	{
		if (e > 30)                                            
			return s | 0x7c00;
		return s | (e << 10) | (f >> 13);
	}
}

uint32_t HalfFloat::halfToFloat() const
{
	int s = (_half >> 15) & 0x00000001;                            
	int e = (_half >> 10) & 0x0000001f;                           
	int f = _half & 0x000003ff;                           

	if (e == 0) 
	{
		if (f == 0)                                            
			return s << 31;
		else 
		{                                                 
			while (!(f & 0x00000400)) 
			{
				f <<= 1;
				e -= 1;
			}
			e += 1;
			f &= ~0x00000400;
		}
	}
	else if (e == 31) 
	{
		if (f == 0)                                             // Inf
			return (s << 31) | 0x7f800000;
		else                                                    // NaN
			return (s << 31) | 0x7f800000 | (f << 13);
	}

	e = e + (127 - 15);
	f = f << 13;

	return ((s << 31) | (e << 23) | f);
}
