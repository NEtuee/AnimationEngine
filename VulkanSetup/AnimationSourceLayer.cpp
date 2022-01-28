#include "AnimationSourceLayer.h"
#include "AnimationLayerBase.h"
#include "AnimationStateMachine.h"
#include "Transform.h"
#include "AnimationStateBase.h"

AnimationSourceLayer::AnimationSourceLayer()
	:_stateMachine(nullptr), _name("")
{
}

void AnimationSourceLayer::createAnimationLayerBase(std::string name)
{
	AnimationLayerBase::createAnimationLayerBase(name);
	_stateMachine = new AnimationStateMachine;
	_stateMachine->createAnimationStateMachine();
}

void AnimationSourceLayer::destroyAnimationLayerBase()
{
	_stateMachine->destroyAnimationStateMachine();
	delete _stateMachine;
}

void AnimationSourceLayer::initialize()
{
	_stateMachine->initialize();
}

void AnimationSourceLayer::addState(AnimationStateBase* state)
{
	_stateMachine->addState(state);
}

void AnimationSourceLayer::deleteState(size_t index)
{
	_stateMachine->deleteState(index);
}

void AnimationSourceLayer::setDefaultState(AnimationStateBase* state)
{
	_stateMachine->setDefaultState(state);
}

void AnimationSourceLayer::setDefaultState(std::string name)
{
	_stateMachine->setDefaultState(name);
}

void AnimationSourceLayer::frame(float deltaTime)
{
	_stateMachine->frame(deltaTime);
}

AnimationStateBase* AnimationSourceLayer::findState(std::string name)
{
	return _stateMachine->findState(name);
}

AnimationStateBase* AnimationSourceLayer::getCurrentState()
{
	return _stateMachine->getCurrentState();
}

const std::vector<AnimationStateBase*>& AnimationSourceLayer::getStates()
{
	return _stateMachine->getStates();
}

Transform AnimationSourceLayer::getCurrentPose(TransformStructure* structure, int& outIndex)
{
	Transform pose = _stateMachine->getCurrentPose(structure, outIndex);
	updateWorldCache(pose, structure);
	return pose;
}
