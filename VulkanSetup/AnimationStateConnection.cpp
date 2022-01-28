#include "AnimationStateConnection.h"
#include "AnimationStateCondition.h"
#include "AnimationStateBase.h"

#include <algorithm>

AnimationStateConnection::AnimationStateConnection()
	:_conditions(), _targetState(nullptr),_blendTime(0.f)
{
}

void AnimationStateConnection::createAnimationStateConnection(AnimationStateBase* state, float blendTime)
{
	_targetState = state;
	_blendTime = blendTime;
}


void AnimationStateConnection::destroyAnimationStateConnection()
{
	for (auto iter = _conditions.begin(); iter != _conditions.end(); ++iter)
	{
		(*iter)->destroyAnimationStateCondition();
		delete (*iter);
	}
}

void AnimationStateConnection::addCondition(AnimationStateVariable* variable, StateConditions type)
{
	auto condition = new AnimationStateCondition;
	condition->createAnimationStateCondition(variable, type);

	addCondition(condition);
}

void AnimationStateConnection::addCondition(AnimationStateCondition* condition)
{
	_conditions.push_back(condition);
}

void AnimationStateConnection::deleteCondition(size_t index)
{
	_conditions.erase(_conditions.begin() + index);
}

const std::vector<AnimationStateCondition*>& AnimationStateConnection::getConditions()
{
	return _conditions;
}

AnimationStateBase* AnimationStateConnection::getTargetState()
{
	return _targetState;
}

float AnimationStateConnection::getBlendTime()
{
	return _blendTime;
}

bool AnimationStateConnection::isEmpty()
{
	return _conditions.size() == 0;
}

bool AnimationStateConnection::isMeet()
{
	for (auto iter = _conditions.begin(); iter != _conditions.end(); ++iter)
	{
		if ((*iter)->isMeet() == false)
			return false;
	}

	return true;
}
