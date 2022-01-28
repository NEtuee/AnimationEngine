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

	void						setStartTime(double time);

	bool isLoop();
	float getAnimationPercentage(double currentTime);

	AnimationDataPack*			getAnimationPack();
	Transform					getCurrentPose(size_t hashedName, double currentTime, int& outIndex);
	Transform					getBlendPoseByTime(size_t hashedName, double currentTime, float factor, const Transform& blendTarget, int& outIndex);
	std::string					getName() { return _name; }
	std::vector<AnimationStateConnection*>& getConnections();
private:
	std::vector<AnimationStateConnection*>	_connections;
	std::string								_name;

	double									_startTime;
	AnimationDataPack*						_animation;
};

