#include "AnimationStateBase.h"
#include "AnimationStateConnection.h"
#include "AnimationDataPack.h"
#include "Transform.h"
#include <cmath>

AnimationStateBase::AnimationStateBase()
	:_startTime(0.0)
{
}

void AnimationStateBase::createAnimationState(std::string name)
{
	_name = name;
	_connections.clear();
}

void AnimationStateBase::destroyAnimationState()
{
	for (auto iter = _connections.begin(); iter != _connections.end(); ++iter)
	{
		(*iter)->destroyAnimationStateConnection();
		delete (*iter);
	}
}

AnimationStateConnection* AnimationStateBase::frame()
{
	for (auto iter = _connections.begin(); iter != _connections.end(); ++iter)
	{
		if ((*iter)->isMeet())
		{
			return (*iter);
		}
	}

	return nullptr;
}

void AnimationStateBase::addConnection(AnimationStateConnection* connection)
{
	_connections.push_back(connection);
}

void AnimationStateBase::deleteConnection(size_t index)
{
	_connections.erase(_connections.begin() + index);
}

void AnimationStateBase::setStartTime(double time)
{
	_startTime = time;
}

Transform AnimationStateBase::getCurrentPose(AnimationDataPack* animation, size_t hashedName, double currentTime, int& outIndex)
{
	double animationTime = (currentTime - _startTime) * animation->getSpeed();
	if (animation->isLoop())
		animationTime = std::fmod(animationTime, animation->getTime());

	return animation->getPoseByTime(outIndex, animationTime, hashedName);
}

Transform AnimationStateBase::getBlendPoseByTime(AnimationDataPack* animation, size_t hashedName, double currentTime, float factor, const Transform& blendTarget, int& outIndex)
{
	Transform pose = getCurrentPose(hashedName, currentTime, outIndex);
	pose = pose.lerp(blendTarget, factor);

	return pose;
}