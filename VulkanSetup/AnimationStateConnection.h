#pragma once
#include <vector>

enum class StateConditions;

class AnimationStateCondition;
class AnimationStateVariable;
class AnimationStateBase;
class AnimationStateConnection
{
public:
							AnimationStateConnection();

	void					createAnimationStateConnection(AnimationStateBase* state, float blendTime);
	void					destroyAnimationStateConnection();

	void					addCondition(AnimationStateVariable* variable, StateConditions type);
	void					addCondition(AnimationStateCondition* condition);
	void					deleteCondition(size_t index);

	const std::vector<AnimationStateCondition*>& getConditions();
	AnimationStateBase*			getTargetState();
	float					getBlendTime();
	bool					isEmpty();
	bool					isMeet();
private:
	std::vector<AnimationStateCondition*>		_conditions;
	AnimationStateBase*								_targetState;
	float										_blendTime;
};

