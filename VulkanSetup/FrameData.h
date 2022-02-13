#pragma once
#include "DirectXDefine.h"
#include "HalfFloat.h"
#include "CompressedQuaternion.h"
#include <iostream>

class Serialization;

class FrameData
{
public:
							FrameData();

	XMVECTOR&				getRotationVector();
	XMVECTOR&				getScaleVector();
	XMVECTOR&				getPositionVector();

	bool					similar(const HalfFloat3& a, const HalfFloat3& b, const float& factor);
	bool					similar(const CompressedQuaternion& a, const CompressedQuaternion& b, const float& factor);
	bool					similar(const FrameData& frame, const float& factor);

	void					setPosition(float x, float y, float z);
	void					setScale(float x, float y, float z);
	void					setRotation(float x, float y, float z, float w);

	void					serialize(Serialization* serialize, std::ostream* stream);
	void					deserialize(Serialization* serialize, std::istream* stream);

	void updateVectors();
public:
	CompressedQuaternion	rotation;
	HalfFloat3				scale;
	HalfFloat3				position;

	unsigned short			frame;
private:
	XMVECTOR				_positionVector;
	XMVECTOR				_rotationVector;
	XMVECTOR				_scaleVector;
};

