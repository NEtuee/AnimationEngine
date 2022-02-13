#include "AnimationStateBlendSpace.h"
#include "AnimationDataPack.h"
#include "AnimationDataPack.h"
#include "Transform.h"
#include "AnimationStateVariableFloat.h"
#include "Triangulation.h"
#include "MathEx.h"
#include <cmath>

void AnimationStateBlendSpace::createAnimationState(std::string name)
{
	AnimationStateBase::createAnimationState(name);

	_triangulation = new Triangulation;

}

void AnimationStateBlendSpace::destroyAnimationState()
{
	AnimationStateBase::destroyAnimationState();

	delete _triangulation;
}

void AnimationStateBlendSpace::createBlendSpace(AnimationStateVariableFloat* row, AnimationStateVariableFloat* column)
{
	_rowWeight = row;
	_columnWeight = column;

	_triangulation->triangulation();
}

void AnimationStateBlendSpace::addAnimationkey(AnimationDataPack* animation, float rowWeight, float columnWeight)
{
	BlendSpaceItem item;
	item._animation = animation;
	item._rowWeight = rowWeight;
	item._columnWeight = columnWeight;

	_items.push_back(item);
	_triangulation->addPoint(rowWeight, columnWeight);
}

bool AnimationStateBlendSpace::isLoop()
{
	return true;
}

float AnimationStateBlendSpace::getAnimationPercentage(float currentTime)
{
	return 0.0f;
}

Transform AnimationStateBlendSpace::getCurrentPose(size_t hashedName, float currentTime, size_t& outIndex)
{
	float animationTime = (currentTime - _startTime);

	auto& triangles = _triangulation->getTriangles();
	auto& points = _triangulation->getPoints();

	float x = *static_cast<float*>(_rowWeight->getVariable());
	float y = *static_cast<float*>(_columnWeight->getVariable());

	XMVECTOR targetPoint = XMVectorSet(x, y, 0.f, 0.f);
	XMVECTOR nearestPoint;
	float nearDist, t, s, ots;
	int edge;

	int nearestTriangle = 0;
	float nearestDist = HUGE_VAL;
	float nearestT = 0.f, nearestOTS = 0.f;


	for (int i = 0; i < triangles.size(); ++i)
	{
		auto a = MathEx::toVector(points[triangles[i]._a]);
		auto b = MathEx::toVector(points[triangles[i]._b]);
		auto c = MathEx::toVector(points[triangles[i]._c]);

		bool find = MathEx::findNearestPointOnTriangle(targetPoint,a,b,c,
			nearestPoint, nearDist, t, s, ots, edge);

		if (find == false)
		{
			_nearestPoint = nearestPoint;
			_pointOut = false;
			return getBarycentricTransformFromTriangle(&triangles[i], hashedName, animationTime, outIndex, t, ots);;
		}
		else
		{
			if (nearestDist > nearDist)
			{
				nearestDist = nearDist;

				_nearestPoint = nearestPoint;
				nearestTriangle = i;
				nearestT = t;
				nearestOTS = ots;
			}
		}
	}

	_pointOut = true;
	return getBarycentricTransformFromTriangle(&triangles[nearestTriangle], hashedName, animationTime, outIndex, nearestT, nearestOTS);
}

Transform AnimationStateBlendSpace::getBlendPoseByTime(size_t hashedName, float currentTime, float factor, const Transform& blendTarget, size_t& outIndex)
{
	Transform pose = getCurrentPose(hashedName, currentTime, outIndex);
	pose = pose.lerp(blendTarget, factor);

	return pose;
}

Transform AnimationStateBlendSpace::getBarycentricTransformFromTriangle(const Triangle* triangle, size_t hashedName, float animationTime, size_t& outIndex, float t,float ots)
{
	size_t blendIndexA, blendIndexB, blendIndexC;

	Transform poseA = _items[triangle->_a]._animation->getPoseByTime(blendIndexA, static_cast<float>(animationTime), hashedName);
	Transform poseB = _items[triangle->_b]._animation->getPoseByTime(blendIndexB, static_cast<float>(animationTime), hashedName);
	Transform poseC = _items[triangle->_c]._animation->getPoseByTime(blendIndexC, static_cast<float>(animationTime), hashedName);

	Transform last = poseA.barycentric(poseB, poseC, t, ots);

	outIndex = blendIndexA * blendIndexB * blendIndexC < 0 ? -1 : 0;
	return last;
}
