#pragma once
#include <string>
#include <vector>

class AnimationDataPack;
class AnimationStateConnection;
class Transform;
class AnimationState
{
public:
								AnimationState();

	void						createAnimationState(AnimationDataPack* animation, std::string name);
	void						destroyAnimationState();

	AnimationStateConnection*	update();

	void						addConnection(AnimationStateConnection* connection);
	void						deleteConnection(size_t index);

	void						setStartTime(float time);

	bool						isLoop();
	float						getAnimationPercentage(float currentTime);

	AnimationDataPack*			getAnimationPack();
	Transform					getCurrentPose(size_t hashedName, float currentTime, size_t& outIndex);
	Transform					getBlendPoseByTime(size_t hashedName, float currentTime, float factor, const Transform& blendTarget, size_t& outIndex);
	std::string					getName() { return _name; }
	std::vector<AnimationStateConnection*>& getConnections();
private:
	std::vector<AnimationStateConnection*>	_connections;
	std::string								_name;

	float									_startTime;
	AnimationDataPack*						_animation;
};

