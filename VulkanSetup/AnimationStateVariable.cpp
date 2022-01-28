#include "AnimationStateVariable.h"

void AnimationStateVariable::deleteVariable()
{

}

StateVariableTypeEnum AnimationStateVariable::getType()
{
	return _type;
}

std::string AnimationStateVariable::getName()
{
	return _name;
}

const char* AnimationStateVariable::typeToString()
{
	switch (_type)
	{
	case StateVariableTypeEnum::Bool:	return "Bool";
	case StateVariableTypeEnum::Float:	return "Float";
	case StateVariableTypeEnum::Int:	return "Int";
	}

	return "";
}
