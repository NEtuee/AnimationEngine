#pragma once
#include "SplineKeyVariable.h"
#include <vector>

class SplineAnimationDataRow
{
private:
	SplineKeyVariable _tx, _ty, _tz;
	SplineKeyVariable _sx, _sy, _sz;
	SplineKeyVariable _r1, _r2, _r3;

	std::vector<unsigned short> _frames;

};

