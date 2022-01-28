#include "AnimationStateMachine.h"
#include "Transform.h"
#include "TransformStructure.h"
#include "AnimationStateBase.h"
#include "AnimationPlayer.h"
#include "AnimationStateConnection.h"
#include "Timer.h"

AnimationStateMachine::AnimationStateMachine()
	:_states(),_defaultState(nullptr),_currentState(nullptr),_targetState(nullptr), _stateChanging(false)
{
}

void AnimationStateMachine::createAnimationStateMachine()
{
	//_player = new AnimationPlayer;
	//_player->initialize();
}

void AnimationStateMachine::destroyAnimationStateMachine()
{
	//_player->destroyAnimationPlayer();
	//delete _player;

	for (auto iter = _states.begin(); iter != _states.end(); ++iter)
	{
		(*iter)->destroyAnimationState();
		delete (*iter);
	}
}

void AnimationStateMachine::initialize()
{
	_currentState = _defaultState;
	_stateChanging = false;
}

void AnimationStateMachine::addState(AnimationStateBase* state)
{
	_states.push_back(state);
}

void AnimationStateMachine::deleteState(size_t index)
{
	_states.erase(_states.begin() + index);
}

void AnimationStateMachine::setDefaultState(AnimationStateBase* state)
{
	_defaultState = state;
	_defaultState->setStartTime(Timer::getInstance()->getGlobalTime());
	//_player->setAnimation(_defaultState->getAnimationPack());
	//_player->initialize();
}

void AnimationStateMachine::setDefaultState(std::string name)
{
	setDefaultState(findState(name));
}

void AnimationStateMachine::frame(float deltaTime)
{
	if (_currentState == nullptr)
		return;

	//_player->playTimer(deltaTime);

	if (!_stateChanging)
	{
		auto connection = _currentState->frame();
		if (connection == nullptr)
		{
			return;
		}

		if (connection->isEmpty() && _currentState->isLoop() == false && _currentState->getAnimationPercentage(Timer::getInstance()->getGlobalTime()) < 0.9f)
			return;

		_stateChanging = true;
		_targetState = connection->getTargetState();
		_blendDuration = connection->getBlendTime();
		_blendTimer = Timer::getInstance()->getGlobalTime();
		_targetState->setStartTime(_blendTimer);


		//_player->startBlend(_targetState->getAnimationPack(), connection->getBlendTime());
	}
	else
	{
		if ((Timer::getInstance()->getGlobalTime() - _blendTimer) >= _blendDuration)
		{
			_stateChanging = false;
			_currentState = _targetState;
			_currentState->setStartTime(_blendTimer);
		}
		//if (_player->isBlending() == false)
		//{
		//	_stateChanging = false;
		//	_currentState = _targetState;
		//}
	}
}

AnimationStateBase* AnimationStateMachine::findState(std::string name)
{
	for (auto iterator = _states.begin(); iterator != _states.end(); ++iterator)
	{
		if ((*iterator)->getName().compare(name) == 0)
			return (*iterator);
	}

	return nullptr;
}

//Transform AnimationStateMachine::getIncreasedTransform(TransformStructure* target)
//{
//	int index;
//	//return _player->getIncreasePerFrameReal(target, index);
//}

AnimationStateBase* AnimationStateMachine::getCurrentState()
{
	return _currentState;
}

const std::vector<AnimationStateBase*>& AnimationStateMachine::getStates()
{
	return _states;
}

Transform AnimationStateMachine::getCurrentPose(TransformStructure* structure, int& outIndex)
{
	if (_currentState == nullptr)
	{
		outIndex = -1;
		return Transform();
	}


	Transform pose;
	if (_stateChanging == false)
	{
		pose = _currentState->getCurrentPose(structure->getHashedName(), Timer::getInstance()->getGlobalTime(), outIndex);
	}
	else
	{
		int blendIndex = 0;
		Transform blendTarget = _targetState->getCurrentPose(structure->getHashedName(), Timer::getInstance()->getGlobalTime(), blendIndex);

		if (blendIndex == -1)
		{
			blendTarget = structure->getLocalTransform();
		}

		double currentTime = Timer::getInstance()->getGlobalTime() - _blendTimer;
		pose = _currentState->getBlendPoseByTime(structure->getHashedName(), Timer::getInstance()->getGlobalTime(), currentTime / _blendDuration, blendTarget, outIndex);

		if (outIndex == -1)
		{
			pose = structure->getLocalTransform();
			outIndex = 0;
		}
	}

	return pose;
	//return _player->getcurrentPose(structure, outIndex);
}
