#pragma once
#include <string>
#include <vector>
#include "AnimationLayerBase.h"

class AnimationStateBase;
class AnimationStateMachine;
class Transform;
class TransformStructure;
class AnimationSourceLayer : public AnimationLayerBase
{
public:
										AnimationSourceLayer();

	virtual void						createAnimationLayerBase(std::string name) override;
	virtual void						destroyAnimationLayerBase() override;

	virtual void						initialize() override;

	void								addState(AnimationStateBase* state);
	void								deleteState(size_t index);

	void								setDefaultState(AnimationStateBase* state);
	void								setDefaultState(std::string name);

	virtual void						frame(float deltaTime) override;

	AnimationStateBase*					findState(std::string name);

	AnimationStateBase*					getCurrentState();
	const std::vector<AnimationStateBase*>& getStates();
	virtual Transform					getCurrentPose(TransformStructure* structure, size_t& outIndex, bool masking = false) override;
private:
	AnimationStateMachine*				_stateMachine;
	std::string							_name;
};

