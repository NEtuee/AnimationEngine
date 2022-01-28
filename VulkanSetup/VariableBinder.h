#pragma once
#include <unordered_map>
#include <string>

enum class VariableType
{
	Bool,
	Int,
	Float,
	String,

};
class VariableBinder
{
public:
	void setVariable(std::string target, std::string data);
protected:
	void bindFloat(std::string name, float* target);
	void bindInt(std::string name, int* target);
	void bindBool(std::string name, bool* target);
	void bindString(std::string name, std::string* target);
private:
	void bind(std::string name, VariableType type, void* target);

	std::unordered_map<std::string, std::pair<VariableType, void*>> _variables;
};

