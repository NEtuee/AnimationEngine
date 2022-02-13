#include "AnimationStateSingle.h"
#include "AnimationDataPack.h"
#include "Transform.h"
#include <cmath>

void AnimationStateSingle::setAnimation(AnimationDataPack* animation)
{
	_animation = animation;
}

bool AnimationStateSingle::isLoop()
{
    return _animation->isLoop();
}

float AnimationStateSingle::getAnimationPercentage(float currentTime)
{
	float percentage = static_cast<float>((currentTime - _startTime) / _animation->getTime());
	//percentage = MathEx::clamp01(percentage);
	return percentage;
}

Transform AnimationStateSingle::getCurrentPose(size_t hashedName, float currentTime, size_t& outIndex)
{
	return AnimationStateBase::getCurrentPose(_animation, hashedName, currentTime, outIndex);
}

Transform AnimationStateSingle::getBlendPoseByTime(size_t hashedName, float currentTime, float factor, const Transform& blendTarget, size_t& outIndex)
{
	return AnimationStateBase::getBlendPoseByTime(_animation, hashedName, currentTime, factor, blendTarget, outIndex);
}
