#include "VariableBinder.h"
#include <cassert>
#include <algorithm>

void VariableBinder::setVariable(std::string target, std::string data)
{
	if (target == "" || data == "")
	{
		assert(false);
		return;
	}

	auto find = _variables.find(target);
	if (find == _variables.end())
	{
		assert(false);
		return;
	}

 	switch ((*find).second.first)
	{
	case VariableType::Bool:
		std::transform(data.cbegin(), data.cend(), data.begin(), std::tolower);
		(*static_cast<bool*>((*find).second.second)) = data.compare("true") == 0;
		break;
	case VariableType::Int:
		(*static_cast<int*>((*find).second.second)) = std::stoi(data);
		break;
	case VariableType::Float:
		(*static_cast<float*>((*find).second.second)) = std::stof(data);
		break;
	case VariableType::String:
		(*static_cast<std::string*>((*find).second.second)) = data;
		break;

	}
}

void VariableBinder::bindFloat(std::string name, float* target)
{
	bind(name, VariableType::Float, target);
}

void VariableBinder::bindInt(std::string name, int* target)
{
	bind(name, VariableType::Int, target);
}

void VariableBinder::bindBool(std::string name, bool* target)
{
	bind(name, VariableType::Bool, target);
}

void VariableBinder::bindString(std::string name, std::string* target)
{
	bind(name, VariableType::String, target);
}

void VariableBinder::bind(std::string name, VariableType type, void* target)
{
	_variables.emplace(name, std::make_pair(type, target));
}
