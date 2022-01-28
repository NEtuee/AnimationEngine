#pragma once
#include "DirectXDefine.h"
#include "HalfFloat.h"
#include <iostream>

class Serialization;

class FrameData
{
public:
	HalfFloat4			rotation;
	HalfFloat3			scale;
	HalfFloat3			position;

	unsigned short		frame;

public:
	XMVECTOR			getRotationVector();
	XMVECTOR			getScaleVector();
	XMVECTOR			getPositionVector();

	bool				similar(const HalfFloat3& a, const HalfFloat3& b, const float& factor);
	bool				similar(const HalfFloat4& a, const HalfFloat4& b, const float& factor);
	bool				similar(const FrameData& frame, const float& factor);

	void				setPosition(float x, float y, float z);
	void				setScale(float x, float y, float z);
	void				setRotation(float x, float y, float z, float w);

	void				serialize(Serialization* serialize, std::ostream* stream);
	void				deserialize(Serialization* serialize, std::istream* stream);
};

