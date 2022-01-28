#pragma once
#include <string>

enum class StateVariableTypeEnum;
enum class StateConditions;

class CharacterSet;
class AnimationStateCondition;
class BlendTree;
class AnimationLayerBase;
class XMLElement;

class CharacterSetLoader
{
public:
	CharacterSet*				loadCharacterSet(std::string path);
	AnimationLayerBase*			createSourceLayer(CharacterSet* character, BlendTree* blendTree, XMLElement* element);
	AnimationLayerBase*			createBlendLayer(CharacterSet* character, BlendTree* blendTree, XMLElement* element, std::string skeletonPath);
	StateVariableTypeEnum		stringToTypeEnum(std::string str);
	AnimationStateCondition*	stringToCondition(std::string str, BlendTree* blendTree);
	StateConditions				findConditionInString(std::string str, std::string& sign);
};

