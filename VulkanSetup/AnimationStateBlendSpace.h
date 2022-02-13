#pragma once
#include "AnimationStateBase.h"
#include "DirectXDefine.h"

struct Triangle;

class AnimationStateVariableFloat;
class AnimationDataPack;
class Triangulation;

struct BlendSpaceItem
{
	AnimationDataPack* _animation;
	float _rowWeight;
	float _columnWeight;
};

class AnimationStateBlendSpace : public AnimationStateBase
{
public:
	virtual void					createAnimationState(std::string name) override;
	virtual void					destroyAnimationState() override;

	void							createBlendSpace(AnimationStateVariableFloat* row, AnimationStateVariableFloat* column);
	void							addAnimationkey(AnimationDataPack* animation, float rowWeight, float columnWeight);


	virtual bool					isLoop() override;
	virtual float					getAnimationPercentage(float currentTime) override;
	virtual Transform				getCurrentPose(size_t hashedName, float currentTime, size_t& outIndex) override;
	virtual Transform				getBlendPoseByTime(size_t hashedName, float currentTime, float factor, const Transform& blendTarget, size_t& outIndex) override;

	bool							isPointOut()			{ return _pointOut; }
	XMVECTOR						getNearestPoint()		{ return _nearestPoint; }
	AnimationStateVariableFloat*	getRowVariable()		{ return _rowWeight; }
	AnimationStateVariableFloat*	getColumnVariable()		{ return _columnWeight; }
	Triangulation*					getTriangle()			{ return _triangulation; }
	const std::vector<BlendSpaceItem>& getItems()			{ return _items; }
private:
	Transform						getBarycentricTransformFromTriangle(const Triangle* triangle, size_t hashedName, float animationTime, size_t& outIndex, float t, float ots);
private:
	AnimationStateVariableFloat*	_rowWeight;
	AnimationStateVariableFloat*	_columnWeight;
	Triangulation*					_triangulation;

	std::vector<BlendSpaceItem>		_items;

	XMVECTOR _nearestPoint;

	bool _pointOut;
};