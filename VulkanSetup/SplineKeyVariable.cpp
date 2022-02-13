#include "SplineKeyVariable.h"
#include <Math.h>

void SplineKeyVariable::addPoint(float value, float time)
{
	if (_spline.getSize() == 0)
	{
		_min = _max = time;
	}
	else if(time < _min)
	{
		_min = time;
	}
	else if (time > _max)
	{
		_max = time;
	}

	_spline.addPoint(XMVectorSet(time, value, 0.f, 0.f));
}

void SplineKeyVariable::insertPoint(float value, float time)
{
	if (_spline.getSize() == 0)
	{
		_min = _max = time;
	}
	else if (time < _min)
	{
		_min = time;
	}
	else if (time > _max)
	{
		_max = time;
	}

	_spline.insertPoint(XMVectorSet(time, value, 0.f, 0.f),findIndexFromTime(time));
}

void SplineKeyVariable::clearPoint()
{
	_spline.clearPoint();
}

float SplineKeyVariable::getValueByPercentage(float per)
{
	float time = percentageToTime(per);
	int index = findIndexFromTime(time) - 1;
	if (index == _spline.getSize() - 1)
	{
		return XMVectorGetY(_spline.getPoint(index));
	}

	float nextTime = XMVectorGetX(_spline.getPoint(index + 1));
	float currTime = XMVectorGetX(_spline.getPoint(index));
	float factor = (time - currTime) / (nextTime - currTime);

	return XMVectorGetY(_spline.getSpline(index, factor));
}

float SplineKeyVariable::getPointTime(int index)
{
	return XMVectorGetX(_spline.getPoint(index));
}

XMVECTOR SplineKeyVariable::getSpline(int index, float factor)
{
	return _spline.getSpline(index, factor);
}

int SplineKeyVariable::getSize()
{
	return _spline.getSize();
}

int SplineKeyVariable::findIndexFromTime(float time)
{
	for (int i = 0; i < _spline.getSize(); ++i)
	{
		if (XMVectorGetX(_spline.getPoint(i)) > time)
			return i;
	}

	return _spline.getSize() - 1;
}

float SplineKeyVariable::percentageToTime(float per)
{
	float between = _max - _min;
	return _min + per * between;
}
