#include "AnimationStateVariableFloat.h"

AnimationStateVariableFloat::AnimationStateVariableFloat()
	:_float(0.f)
{
}

void AnimationStateVariableFloat::createVariable(std::string name, void* value)
{
	_name = name;
	setVariable(value);
	_type = StateVariableTypeEnum::Float;
}

void AnimationStateVariableFloat::deleteVariable()
{
}

size_t AnimationStateVariableFloat::getDefaultSize()
{
	return sizeof(float);
}

void* AnimationStateVariableFloat::getVariable()
{
	return &_float;
}

void AnimationStateVariableFloat::setVariable(void* value)
{
	if (value == nullptr)
		return;
	_float = *static_cast<float*>(value);
}

bool AnimationStateVariableFloat::greaterThen(void*& target)
{
	float value = static_cast<float>(*static_cast<char*>(target));

	return _float > value;
}

bool AnimationStateVariableFloat::smallerThen(void*& target)
{
	float value = static_cast<float>(*static_cast<char*>(target));

	return _float < value;
}

bool AnimationStateVariableFloat::greaterEqualThen(void*& target)
{
	float value = static_cast<float>(*static_cast<char*>(target));

	return _float >= value;
}

bool AnimationStateVariableFloat::smallerEqualThen(void*& target)
{
	float value = static_cast<float>(*static_cast<char*>(target));

	return _float <= value;
}

bool AnimationStateVariableFloat::equal(void*& target)
{
	float value = static_cast<float>(*static_cast<char*>(target));

	return _float == value;
}

void AnimationStateVariableFloat::setStringToVariable(std::string str)
{
	_float = std::stof(str);
}

char* AnimationStateVariableFloat::stringToVariable(std::string str)
{
	char* var = new char[sizeof(float)];
	float value = std::stof(str);
	memcpy(var, &value, sizeof(float));

	return var;
}
