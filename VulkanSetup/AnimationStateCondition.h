#pragma once

enum class StateConditions
{
	Equal,
	GreaterEqual,
	SmallerEqual,
	Greater,
	Smaller,
	True,
	False,
};

class AnimationStateVariable;
class AnimationStateCondition
{
public:
								AnimationStateCondition();

	void						createAnimationStateCondition(AnimationStateVariable* var, StateConditions condition);
	void						destroyAnimationStateCondition();

	bool						isMeet();

	void						createTarget(size_t size);

	template<typename t>
	void						setTarget(const t& target);

	StateConditions				getCondition();
	AnimationStateVariable*		getVariable();
	const char*					conditionToString();
	const char*					getTarget();
private:
	AnimationStateVariable*		_var;
	StateConditions				_condition;
	char*						_targetVariable;
};

template<typename t>
inline void AnimationStateCondition::setTarget(const t& target)
{
	int i = sizeof(t);

	memcpy(_targetVariable, &target, sizeof(t));
}
