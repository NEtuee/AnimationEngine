#include "AnimationStateCondition.h"
#include "AnimationStateVariable.h"
#include <windows.h>

AnimationStateCondition::AnimationStateCondition()
	:_var(nullptr),_condition(StateConditions::Equal), _targetVariable(nullptr)
{

}

void AnimationStateCondition::createAnimationStateCondition(AnimationStateVariable* var, StateConditions condition)
{
	_var = var;
	_targetVariable = nullptr;
	_condition = condition;

	createTarget(var->getDefaultSize());
}

void AnimationStateCondition::destroyAnimationStateCondition()
{
	//if(_targetVariable != nullptr)
	//	delete _targetVariable;
	//delete _var;
}

bool AnimationStateCondition::isMeet()

{
	void* pointer = static_cast<void*>(_targetVariable);
	switch (_condition)
	{
	case StateConditions::Equal:		return _var->equal(pointer);
	case StateConditions::Greater:		return _var->greaterThen(pointer);
	case StateConditions::GreaterEqual:	return _var->greaterEqualThen(pointer);
	case StateConditions::Smaller:		return _var->smallerThen(pointer);
	case StateConditions::SmallerEqual:	return _var->smallerEqualThen(pointer);
	case StateConditions::True:			return *static_cast<bool*>(_var->getVariable()) == true;
	case StateConditions::False:		return *static_cast<bool*>(_var->getVariable()) == false;
	}

	return false;
}

void AnimationStateCondition::createTarget(size_t size)
{
	if (_targetVariable != nullptr)
	{
		delete[] _targetVariable;
	}

	_targetVariable = new char[size];
	ZeroMemory(_targetVariable, size);
}

StateConditions AnimationStateCondition::getCondition()
{
	return _condition;
}

AnimationStateVariable* AnimationStateCondition::getVariable()
{
	return _var;
}

const char* AnimationStateCondition::conditionToString()
{
	switch (_condition)
	{
	case StateConditions::Equal:		return "Equal";
	case StateConditions::Greater:		return "Greater";
	case StateConditions::GreaterEqual:	return "GreaterEqual";
	case StateConditions::Smaller:		return "Smaller";
	case StateConditions::SmallerEqual: return "SmallerEqual";
	case StateConditions::True:			return "True";
	case StateConditions::False:		return "False";
	}

	return "";
}

const char* AnimationStateCondition::getTarget()
{
	return _targetVariable;
}
