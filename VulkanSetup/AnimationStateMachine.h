﻿#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class AnimationStateBase;
class AnimationPlayer;
class Transform;
class TransformStructure;

class AnimationStateMachine
{
public:
											AnimationStateMachine();

	void									createAnimationStateMachine();
	void									destroyAnimationStateMachine();

	void									initialize();

	void									addState(AnimationStateBase* state);
	void									deleteState(size_t index);

	void									setDefaultState(AnimationStateBase* state);
	void									setDefaultState(std::string name);

	void									frame(float deltaTime);

	AnimationStateBase*						findState(std::string name);
	AnimationStateBase*						getCurrentState();
	const std::vector<AnimationStateBase*>&	getStates();
	Transform								getCurrentPose(TransformStructure* structure, size_t& outIndex);
private:
	std::vector<AnimationStateBase*>		_states;
	AnimationStateBase*						_defaultState;
	AnimationStateBase*						_currentState;
	AnimationStateBase*						_targetState;

	float									_blendTimer;
	float									_blendDuration;
	bool									_stateChanging;
};

