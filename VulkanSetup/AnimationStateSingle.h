#pragma once
#include "AnimationStateBase.h"

class AnimationDataPack;
class AnimationStateSingle : public AnimationStateBase
{
public:
	void						setAnimation(AnimationDataPack* animation);

	virtual bool				isLoop() override;
	virtual float				getAnimationPercentage(double currentTime) override;
	virtual Transform			getCurrentPose(size_t hashedName, double currentTime, int& outIndex) override;
	virtual Transform			getBlendPoseByTime(size_t hashedName, double currentTime, float factor, const Transform& blendTarget, int& outIndex) override;
private:
	AnimationDataPack* _animation;
};

