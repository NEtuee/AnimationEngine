﻿#include "BlendTree.h"
#include "AnimationStateVariableBoolean.h"
#include "AnimationStateVariableFloat.h"
#include "AnimationLayerBase.h"
#include "Transform.h"
#include "TransformStructure.h"
#include "BoneStructure.h"
#include "MathEx.h"

#include <cassert>


BlendTree::BlendTree()
	:_resultLayer(nullptr), _variables(), _targetBone(nullptr), _prevRoot(nullptr), _rootBone(nullptr), _rootMotion(false)
{
}

void BlendTree::createBlendTree(BoneStructure* bone)
{
	_targetBone = bone;
	_prevRoot = new Transform;
}

void BlendTree::destroyBlendTree()
{
	_resultLayer->destroyAnimationLayerBase();

	for (auto iter = _variables.begin(); iter != _variables.end(); ++iter)
	{
		(*iter).second->deleteVariable();
		delete (*iter).second;
	}

	_variables.clear();
}

void BlendTree::initialize()
{
	_resultLayer->initialize();

}

void BlendTree::frame(float deltaTime)
{
	_resultLayer->frame(deltaTime);


	playAnimation(&_targetBone->getRootBone());
	_targetBone->getRootBone().updateWorld();
}


AnimationStateVariable* BlendTree::createVariable(StateVariableTypeEnum type, std::string name, void* initialValue)
{
	AnimationStateVariable* variable = nullptr;

	switch (type)
	{
	case StateVariableTypeEnum::Bool:
		variable = new AnimationStateVariableBoolean;
		break;
	case StateVariableTypeEnum::Float:
		variable = new AnimationStateVariableFloat;
		break;
	case StateVariableTypeEnum::Int:
		break;
	}

	if (variable == nullptr)
	{
		assert(false);
	}

	variable->createVariable(name, initialValue);

	_variables.emplace(name, variable);

	return variable;
}

void BlendTree::setRootMotion(bool enable, TransformStructure* rootBone)
{
	_rootMotion = enable;
	_rootBone = rootBone;
}

AnimationLayerBase* BlendTree::getLayer() const
{
	return _resultLayer;
}

void BlendTree::setResultLayer(AnimationLayerBase* layer)
{
	_resultLayer = layer;
}

const std::unordered_map<std::string, AnimationStateVariable*>& BlendTree::getVariables() const
{
	return _variables;
}

AnimationStateVariable* BlendTree::getVariable(std::string name)
{
	auto iter = _variables.find(name);
	if (iter != _variables.end())
	{
		return (*iter).second;
	}

	return nullptr;
}

void BlendTree::playAnimation(TransformStructure* root)
{
	int outIndex;
	Transform pose = _resultLayer->getCurrentPose(root, outIndex);
	if(outIndex != -1)
		root->SetLoaclTransformNoCalc(pose.getPosition(), pose.getScale(), pose.getRotation());

	auto& children = root->getChildren();
	for (auto iter = children.begin(); iter != children.end(); ++iter)
	{
		playAnimation((*iter));
	}

	_resultLayer->afterLoop(root);


}

void BlendTree::updateRootMotion()
{
	(*_prevRoot) = _rootBone->getLocalTransform();
}
