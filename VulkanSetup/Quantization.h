#pragma once

namespace Quantization
{
	unsigned int	compressFloatMinMax(float value, float min, float max, unsigned int bits);
	float			decompressFloatMinMax(unsigned int quantized, float min, float max, unsigned int bits);

	unsigned int	compressFloat(float unit, unsigned int bits);
	float			decompressFloat(unsigned int quantized, unsigned int bits);
}

