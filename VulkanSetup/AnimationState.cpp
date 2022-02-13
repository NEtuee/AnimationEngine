#include "AnimationState.h"
#include "AnimationStateConnection.h"
#include "AnimationDataPack.h"
#include "Transform.h"
#include <cmath>

AnimationState::AnimationState()
	:_connections(), _name(""), _animation(nullptr), _startTime(0.f)
{
}

void AnimationState::createAnimationState(AnimationDataPack* animation, std::string name)
{
	_animation = animation;
	_name = name;

	_connections.clear();
}

void AnimationState::destroyAnimationState()
{
	for (auto iter = _connections.begin(); iter != _connections.end(); ++iter)
	{
		(*iter)->destroyAnimationStateConnection();
		delete (*iter);
	}
}

AnimationStateConnection* AnimationState::update()
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

void AnimationState::addConnection(AnimationStateConnection* connection)
{
	_connections.push_back(connection);
}

void AnimationState::deleteConnection(size_t index)
{
	_connections.erase(_connections.begin() + index);
}

void AnimationState::setStartTime(float time)
{
	_startTime = time;
}

bool AnimationState::isLoop()
{
	return _animation->isLoop();
}

float AnimationState::getAnimationPercentage(float currentTime)
{
	float percentage = static_cast<float>((currentTime - _startTime) / _animation->getTime());
	//percentage = MathEx::clamp01(percentage);
	return percentage;
}

AnimationDataPack* AnimationState::getAnimationPack()
{
	return _animation;
}

Transform AnimationState::getCurrentPose(size_t hashedName, float time, size_t& outIndex)
{
	float animationTime = (time - _startTime) * _animation->getSpeed();
	if (_animation->isLoop())
		animationTime = std::fmod(animationTime, _animation->getTime());

	return _animation->getPoseByTime(outIndex, animationTime, hashedName);
}

Transform AnimationState::getBlendPoseByTime(size_t hashedName, float currentTime, float factor, const Transform& blendTarget, size_t& outIndex)
{
	Transform pose = getCurrentPose(hashedName, currentTime, outIndex);
	pose = pose.lerp(blendTarget, factor);

	return pose;
}

std::vector<AnimationStateConnection*>& AnimationState::getConnections()
{
	return _connections;
}
