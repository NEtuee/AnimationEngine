#pragma once
#include "CatmullRomSpline.h"
#include "../VulkanSetup/DirectXDefine.h"

class SplineKeyVariable
{
public:
	void		addPoint(float value, float time);
	void		insertPoint(float value, float time);

	void		clearPoint();

	float		getValueByPercentage(float per);
	float		getPointTime(int index);

	XMVECTOR	getSpline(int index, float factor);
	int			getSize();
private:
	int			findIndexFromTime(float time);
	float		percentageToTime(float per);
private:
	float		_min;
	float		_max;
	CatmullRomSpline _spline;
};

