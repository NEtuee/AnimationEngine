#include "Quantization.h"

unsigned int Quantization::compressFloat(float unit, unsigned int bits)
{
	unsigned int intervals = 1u << bits;
	float scaled = unit * static_cast<float>(intervals - 1u);
	unsigned int rounded = static_cast<unsigned int>(scaled + 0.5f);
	if (rounded > intervals - 1u)
	{
		rounded = intervals - 1u;
	}

	return rounded;
}

float Quantization::decompressFloat(unsigned int quantized, unsigned int bits)
{
	unsigned int intervals = 1u << bits;
	float intervalSize = 1.0f / static_cast<float>(intervals - 1u);
	float approxUnitFloat = static_cast<float>(quantized * intervalSize);
	return approxUnitFloat;
}

unsigned int Quantization::compressFloatMinMax(float value, float min, float max, unsigned int bits)
{
	float unit = (value - min) / (max - min);
	unsigned int quantized = compressFloat(unit, bits);
	return quantized;
}

float Quantization::decompressFloatMinMax(unsigned int quantized, float min, float max, unsigned int bits)
{
	float unit = decompressFloat(quantized, bits);
	float value = min + (unit * (max - min));
	return value;
}
