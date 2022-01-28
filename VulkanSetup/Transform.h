#pragma once
#include "DirectXDefine.h"

class Transform
{
public:
					Transform();
					Transform(const Transform&);

	void			setPosition(const XMFLOAT3& p);
	void			setScale(const XMFLOAT3& s);
	void			setRotation(const XMFLOAT3& degree);

	void			setPosition(const XMVECTOR& p);
	void			setScale(const XMVECTOR& s);
	void			setRotation(const XMVECTOR& quat);

	void			setTransform(const XMVECTOR& p, const XMVECTOR& s, const XMVECTOR& r);

	XMVECTOR		getForward() const;
	XMVECTOR		getRight() const;
	XMVECTOR		getUp() const;

	XMVECTOR		getPosition() const;
	XMVECTOR		getScale() const;
	XMVECTOR		getRotation() const;

	XMMATRIX		getMatrix() const;
	Transform		inverse() const;

	Transform		lerp(const Transform& target, float factor) const;
	Transform		barycentric(const Transform& t2, const Transform& t3, float f, float g);
	Transform		add(const Transform& target) const;
	Transform		sub(const Transform& target);
	Transform		worldToLocal(const Transform& parent);
	Transform		localToWorld(const Transform& parent);
private:
	XMVECTOR		_position;
	XMVECTOR		_scale;
	XMVECTOR		_rotation;
};

