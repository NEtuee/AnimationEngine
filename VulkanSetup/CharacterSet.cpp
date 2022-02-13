#include "CharacterSet.h"
#include "BoneStructure.h"
#include "BlendTree.h"
#include "AnimationDataPack.h"
#include "IKChain.h"
#include "IKSolver.h"
#include "FABRIKSolver.h"
#include "CCDIKSolver.h"
#include "AnalyticTwoBoneIKSolver.h"
#include "TransformStructure.h"

CharacterSet::CharacterSet()
	:_animations(),_name(""),_bone(nullptr),_tree(nullptr)
{
}

void CharacterSet::createCharacterSet(std::string name)
{
	setName(name);

}

void CharacterSet::destroyCharacterSet()
{
	_tree->destroyBlendTree();
	_bone->destroyBoneStructure();

	for (auto iter = _ikChains.begin(); iter != _ikChains.end(); ++iter)
	{
		(*iter)->destroyIKChain();
		delete (*iter);
	}
}

void CharacterSet::frame(float deltaTime)
{
	_tree->frame(deltaTime);

	for (auto iter = _ikChains.begin(); iter != _ikChains.end(); ++iter)
	{
		(*iter)->solve();
	}
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

const std::vector<IKChain*>& CharacterSet::getIKChains()
{
	return _ikChains;
}

BoneStructure* CharacterSet::getBoneStructure()
{
	return _bone;
}

const BlendTree* CharacterSet::getBlendTree()
{
	return _tree;
}
