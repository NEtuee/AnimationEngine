#pragma once
#include <string>
#include <vector>

class AnimationDataPack;
class AnimationStateConnection;
class Transform;
class AnimationStateBase
{
public:
								AnimationStateBase();

	virtual void				createAnimationState(std::string name);
	virtual void				destroyAnimationState();

	AnimationStateConnection*	frame();

	void						addConnection(AnimationStateConnection* connection);
	void						deleteConnection(size_t index);

	void						setStartTime(double time);

	virtual bool				isLoop() = 0;
	virtual float				getAnimationPercentage(double currentTime) = 0;
	virtual Transform			getCurrentPose(size_t hashedName, double currentTime, int& outIndex) = 0;
	virtual Transform			getBlendPoseByTime(size_t hashedName, double currentTime, float factor, const Transform& blendTarget, int& outIndex) = 0;

	std::string					getName() { return _name; }
	std::vector<AnimationStateConnection*>& getConnections() { return _connections; }
protected:
	Transform					getCurrentPose(AnimationDataPack* animation, size_t hashedName, double currentTime, int& outIndex);
	Transform					getBlendPoseByTime(AnimationDataPack* animation, size_t hashedName, double currentTime, float factor, const Transform& blendTarget, int& outIndex);
	double									_startTime;
private:
	std::vector<AnimationStateConnection*>	_connections;
	std::string								_name;
};

