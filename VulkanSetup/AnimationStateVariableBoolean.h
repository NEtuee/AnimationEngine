#pragma once
#include "AnimationStateVariable.h"

class AnimationStateVariableBoolean : public AnimationStateVariable
{
public:
							AnimationStateVariableBoolean();

	virtual void			createVariable(std::string name, void* value) override;
	virtual void			deleteVariable() override;

	virtual size_t			getDefaultSize()						override;
	virtual void*			getVariable()							override;
	virtual void			setVariable(void* value)				override;
	virtual bool			greaterThen(void*& target)				override;
	virtual bool			smallerThen(void*& target)				override;
	virtual bool			greaterEqualThen(void*& target)			override;
	virtual bool			smallerEqualThen(void*& target)			override;
	virtual bool			equal(void*& target)					override;
	virtual void			setStringToVariable(std::string str)	override;
	virtual char*			stringToVariable(std::string str)		override;
private:
	bool					_boolean;
};

