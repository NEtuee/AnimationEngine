#include "FrameData.h"
#include "MathEx.h"
#include "Serialize.h"

const extern char		positionFlag;
const extern char		scaleFlag;
const extern char		rotationFlag;

XMVECTOR FrameData::getRotationVector()
{
	return XMQuaternionNormalize(XMVectorSet(rotation._x, rotation._y, rotation._z, rotation._w));
}

XMVECTOR FrameData::getScaleVector()
{
	return XMVectorSet(scale._x, scale._y, scale._z, 0.f);
}

XMVECTOR FrameData::getPositionVector()
{
	return XMVectorSet(position._x, position._y, position._z, 0.f);
}

bool FrameData::similar(const HalfFloat3& a, const HalfFloat3& b, const float& factor)
{
	return (MathEx::similar(a._x, b._x, factor) &&
		MathEx::similar(a._y, b._y, factor) &&
		MathEx::similar(a._z, b._z, factor));
}

bool FrameData::similar(const HalfFloat4& a, const HalfFloat4& b, const float& factor)
{
	return (MathEx::similar(a._x, b._x, factor) &&
		MathEx::similar(a._y, b._y, factor) &&
		MathEx::similar(a._z, b._z, factor));
}

bool FrameData::similar(const FrameData& frame, const float& factor)
{
	return similar(frame.scale, scale, factor) &&
		similar(frame.position, position, factor) &&
		similar(frame.rotation, rotation, factor);
}

void FrameData::setPosition(float x, float y, float z)
{
	//_debugPosition.x = x;
	//_debugPosition.y = y;
	//_debugPosition.z = z;

	position._x = x; position._y = y; position._z = z;
}

void FrameData::setScale(float x, float y, float z)
{
	//_debugScale.x = x;
	//_debugScale.y = y;
	//_debugScale.z = z;

	scale._x = x; scale._y = y; scale._z = z;
}

void FrameData::setRotation(float x, float y, float z, float w)
{
	//_debugRotation.x = x;
	//_debugRotation.y = y;
	//_debugRotation.z = z;
	//_debugRotation.w = w;

	rotation._x = x; rotation._y = y; rotation._z = z; rotation._w = w;
}

void FrameData::serialize(Serialization* serialize, std::ostream* stream)
{
	char pos = 0;
	char rot = 0;
	char scl = 0;

	if (similar(position, HalfFloat3::zero(), 0.f))
		pos = positionFlag;
	if (similar(scale, HalfFloat3::one(), 0.f))
		scl = scaleFlag;
	if (similar(rotation, HalfFloat4::quaternionIdentity(), 0.f))
		rot = rotationFlag;

	char flag = pos + scl + rot;
	
	serialize->write(stream, &flag, sizeof(flag));
	if (scl == 0)
		serialize->write(stream, &scale, sizeof(scale));
	if (pos == 0)
		serialize->write(stream, &position, sizeof(position));
	if (rot == 0)
		serialize->write(stream, &rotation, sizeof(rotation));
	serialize->write(stream, &frame, sizeof(frame));
}

void FrameData::deserialize(Serialization* serialize, std::istream* stream)
{
	char flags = 0;

	serialize->read(stream, &flags, sizeof(flags));

	if ((flags & scaleFlag) == 0)
		serialize->read(stream, &scale, sizeof(scale));
	else
		scale = HalfFloat3{ 1,1,1 };
	if ((flags & positionFlag) == 0)
		serialize->read(stream, &position, sizeof(position));
	else
		position = { 0,0,0 };
	if ((flags & rotationFlag) == 0)
		serialize->read(stream, &rotation, sizeof(rotation));
	else
		rotation = { 0,0,0,1 };
	serialize->read(stream, &frame, sizeof(frame));

}