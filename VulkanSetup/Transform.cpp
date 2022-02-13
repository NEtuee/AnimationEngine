#include "Transform.h"
#include "MathEx.h"
#include <iostream>

Transform::Transform()
	:_position(XMVectorSet(0.f,0.f,0.f,0.f)), _rotation(XMQuaternionIdentity()), _scale(XMVectorSet(1.f,1.f,1.f,0.f))
{
}

Transform::Transform(const Transform& target)
	: _position(target.getPosition()), _rotation(target.getRotation()), _scale(target.getScale())
{

}

void Transform::setPosition(const XMFLOAT3& p)
{
	_position = XMVectorSet(p.x,p.y,p.z,0.f);
}

void Transform::setScale(const XMFLOAT3& s)
{
	_scale = XMVectorSet(s.x, s.y, s.z, 0.f);
}

void Transform::setRotation(const XMFLOAT3& degree)
{
	_rotation = XMQuaternionRotationRollPitchYaw(degree.x, degree.y, degree.z);
}

void Transform::setRotation(const XMVECTOR& r)
{
	_rotation = r;
}

void Transform::setTransform(const XMVECTOR& p, const XMVECTOR& s, const XMVECTOR& r)
{
	_position = p;
	_scale = s;
	_rotation = r;
}

void Transform::setPosition(const XMVECTOR& p)
{
	_position = p;
}

void Transform::setScale(const XMVECTOR& s)
{
	_scale = s;
}

XMVECTOR Transform::getForward() const
{
	XMFLOAT3 forward{0.0f,0.0f,1.0f};
	return XMVector3Rotate(XMVectorSet(forward.x, forward.y, forward.z, 0.f), _rotation);
}

XMVECTOR Transform::getRight() const
{
	XMFLOAT3 forward{ 1.0f,0.0f,0.0f };
	return XMVector3Rotate(XMVectorSet(forward.x, forward.y, forward.z, 0.f), _rotation);
}

XMVECTOR Transform::getUp() const
{
	XMFLOAT3 forward{ 0.0f,1.0f,0.0f };
	return XMVector3Rotate(XMVectorSet(forward.x, forward.y, forward.z, 0.f), _rotation);
}

const XMVECTOR& Transform::getPosition() const
{
	return _position;
}

const XMVECTOR& Transform::getScale() const
{
	return _scale;
}

const XMVECTOR& Transform::getRotation() const
{
	return _rotation;
}

XMMATRIX Transform::getMatrix() const
{
	XMMATRIX matrix = XMMatrixIdentity();

	matrix = XMMatrixScalingFromVector(_scale) * XMMatrixRotationQuaternion(_rotation) * XMMatrixTranslationFromVector(_position);
	return matrix;
}

Transform Transform::inverse() const
{
	XMFLOAT3 inverseScale = { 0,0,0 };
	XMFLOAT3 scale;
	XMStoreFloat3(&scale, getScale());

	if (scale.x != 0.f) inverseScale.x = 1.f / scale.x;
	if (scale.y != 0.f) inverseScale.y = 1.f / scale.y;
	if (scale.z != 0.f) inverseScale.z = 1.f / scale.z;

	Transform result;	
	result.setScale(inverseScale);
	result.setRotation(XMQuaternionInverse(_rotation));
	result.setPosition(XMVector3Rotate(XMVectorMultiply(-_position,result.getScale()), result.getRotation()));

	return result;
}

Transform Transform::lerp(const Transform& target, float factor) const
{
	Transform lerped;
	lerped.setTransform(XMVectorLerp(_position, target._position, factor),
		XMVectorLerp(_scale, target._scale, factor),
		XMQuaternionSlerp(_rotation, target._rotation, factor));

	return lerped;
}

Transform Transform::barycentric(const Transform& t2, const Transform& t3, float f, float g) const
{
	Transform bc;
	bc.setTransform(XMVectorBaryCentric(t3._position, _position, t2._position, f, g),
		XMVectorBaryCentric(t3._scale, _scale, t2._scale, f, g),
		XMQuaternionBaryCentric(t3._rotation, _rotation, t2._rotation, f, g));

	return bc;
}

Transform Transform::add(const Transform& target) const
{
	Transform added;
	added.setTransform(XMVectorAdd(_position, target._position),
		XMVectorAdd(_scale, target._scale),
		XMQuaternionMultiply(_rotation, target._rotation));

	return added;
}

Transform Transform::sub(const Transform& target) const
{
	Transform subtracted;
	subtracted.setTransform(XMVectorSubtract(_position, target._position),
		XMVectorSubtract(_scale, target._scale),
		XMQuaternionMultiply(_rotation, XMQuaternionInverse(target._rotation)));

	return subtracted;
}

Transform Transform::worldToLocal(const Transform& parent) const
{
	Transform inverseParent = parent.inverse();
	Transform result;
	result.setScale(XMVectorMultiply(inverseParent.getScale(), _scale));
	result.setRotation(XMQuaternionMultiply(_rotation, inverseParent.getRotation()));
	result.setPosition(
		XMVectorAdd(XMVector3Rotate(XMVectorMultiply(_position, inverseParent.getScale()), inverseParent.getRotation())
			, inverseParent.getPosition()));
	//result.setPosition(XMVector3Rotate(XMVectorAdd(_position, inverseParent.getPosition()),inverseParent.getRotation()));

	return result;
}

Transform Transform::localToWorld(const Transform& parent) const
{
	Transform result;
	result.setRotation(XMQuaternionMultiply(_rotation, parent.getRotation()));
	result.setScale(XMVectorMultiply(parent.getScale(), _scale));
	result.setPosition(
		XMVectorAdd(XMVector3Rotate(XMVectorMultiply(_position, parent.getScale()), parent.getRotation())
		, parent.getPosition()));
	
	return result;
}