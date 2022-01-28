#include "Triangle.h"

bool Triangle::isInCircum(myPoint pt)
{
	auto point = Math::findCircumCenter(_a,_b,_c);
	auto rad = Math::findCircumRadius(_a, _b, _c);

	//return Math::distance(point, pt) < rad;
	return false;
}
