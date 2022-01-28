#include "CharacterSet.h"
#include "BoneStructure.h"
#include "BlendTree.h"
#include "AnimationDataPack.h"

CharacterSet::CharacterSet()
	:_animations(),_name(""),_bone(nullptr),_tree(nullptr)
{
}

void CharacterSet::createCharacterSet(std::string name)
{
	setName(name);
}

void CharacterSet::deleteCharacterSet()
{
	_tree->destroyBlendTree();
}

void CharacterSet::frame(float deltaTime)
{
	_tree->frame(deltaTime);
}

void CharacterSet::setName(std::string name)
{
	_name = name;
}

const std::unordered_map<std::string, AnimationDataPack*>& CharacterSet::getAnimations()
{
	return _animations;
}

const std::string& CharacterSet::getName()
{
	return _name;
}

BoneStructure* CharacterSet::getBoneStructure()
{
	return _bone;
}

const BlendTree* CharacterSet::getBlendTree()
{
	return _tree;
}
