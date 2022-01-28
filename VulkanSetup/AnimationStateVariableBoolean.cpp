#include "AnimationStateVariableBoolean.h"
#include <cassert>

AnimationStateVariableBoolean::AnimationStateVariableBoolean()
	:_boolean(false)
{
}

void AnimationStateVariableBoolean::createVariable(std::string name, void* value)
{
	_name = name;
	setVariable(value);
	_type = StateVariableTypeEnum::Bool;
}

void AnimationStateVariableBoolean::deleteVariable()
{
}

size_t AnimationStateVariableBoolean::getDefaultSize()
{
	return sizeof(bool);
}

void* AnimationStateVariableBoolean::getVariable()
{
	return &_boolean;
}

void AnimationStateVariableBoolean::setVariable(void* value)
{
	if (value == nullptr)
		return;
	_boolean = *static_cast<char*>(value);
}

bool AnimationStateVariableBoolean::greaterThen(void*& target)
{
	assert(false);
	return false;
}

bool AnimationStateVariableBoolean::smallerThen(void*& target)
{
	assert(false);
	return false;
}

bool AnimationStateVariableBoolean::greaterEqualThen(void*& target)
{
	assert(false);
	return false;
}

bool AnimationStateVariableBoolean::smallerEqualThen(void*& target)
{
	assert(false);
	return false;
}

bool AnimationStateVariableBoolean::equal(void*& target)
{
	bool check = static_cast<bool>(*(static_cast<char*>(target)));
	return _boolean == check;
	//return check == _boolean;
}

void AnimationStateVariableBoolean::setStringToVariable(std::string str)
{
	_boolean = (str.compare("True") == 0) ? 1 : 0;
}

char* AnimationStateVariableBoolean::stringToVariable(std::string str)
{
	char* var = new char[sizeof(bool)];
	bool value = (str.compare("true") == 0) ? 1 : 0;
	memcpy(var, &value, sizeof(bool));

	return var;
}
