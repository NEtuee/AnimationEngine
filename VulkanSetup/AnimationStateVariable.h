#pragma once
#include <string>

enum class StateVariableTypeEnum
{
	Int,
	Float,
	Bool,
};

//템플릿으로 바꿀 수 있지 않음??
class AnimationStateVariable
{
public:
	virtual void			createVariable(std::string name, void* value)	= 0;
	virtual void			deleteVariable()								;

	virtual size_t			getDefaultSize()								= 0;
	virtual void*			getVariable()									= 0;
	virtual void			setVariable(void* value)						= 0;
	virtual bool			greaterThen(void*& target)						= 0;
	virtual bool			smallerThen(void*& target)						= 0;
	virtual bool			greaterEqualThen(void*& target)					= 0;
	virtual bool			smallerEqualThen(void*& target)					= 0;
	virtual bool			equal(void*& target)							= 0;
	virtual void			setStringToVariable(std::string)				= 0;
	virtual char*			stringToVariable(std::string)					= 0;


	StateVariableTypeEnum	getType();
	std::string				getName();
	const char*				typeToString();
protected:
	StateVariableTypeEnum	_type;
	std::string				_name;
};

