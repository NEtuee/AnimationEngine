#pragma once
#include "AnimationStateBase.h"

class AnimationDataPack;
class AnimationStateSingle : public AnimationStateBase
{
public:
	void						setAnimation(AnimationDataPack* animation);

	virtual bool				isLoop() override;
	virtual float				getAnimationPercentage(float currentTime) override;
	virtual Transform			getCurrentPose(size_t hashedName, float currentTime, size_t& outIndex) override;
	virtual Transform			getBlendPoseByTime(size_t hashedName, float currentTime, float factor, const Transform& blendTarget, size_t& outIndex) override;
private:
	AnimationDataPack*			_animation;
};

