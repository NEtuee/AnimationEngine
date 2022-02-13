#include "AnimationStateLayer.h"
#include "Transform.h"
#include "TransformStructure.h"
#include "AnimationDataPack.h"
#include "AnimationStateBase.h"
#include "AnimationPlayer.h"
#include "AnimationStateConnection.h"
#include "AnimationStateMachine.h"
#include "MathEx.h"

AnimationStateLayer::AnimationStateLayer()
	:_stateMachine(nullptr), _name(""), _partialMaskRoot(nullptr), _additiveBasePose(nullptr), _weight(1.f), _maskDepth(0.f),
	_partialSmoothCount(0.f),_masking(false), _additive(false)
{

}

void AnimationStateLayer::createAnimationStateLayer(std::string name, float weight)
{
	_name = name;
	setWeight(weight);

	_stateMachine = new AnimationStateMachine;
	_stateMachine->createAnimationStateMachine();
}

void AnimationStateLayer::destroyAnimationStateLayer()
{
	_stateMachine->destroyAnimationStateMachine();
}

void AnimationStateLayer::initialize()
{
	_stateMachine->initialize();
}

void AnimationStateLayer::addState(AnimationStateBase* state)
{
	_stateMachine->addState(state);
}

void AnimationStateLayer::deleteState(size_t index)
{
	_stateMachine->deleteState(index);
}

void AnimationStateLayer::setDefaultState(AnimationStateBase* state)
{
	_stateMachine->setDefaultState(state);
}

void AnimationStateLayer::setDefaultState(std::string name)
{
	_stateMachine->setDefaultState(name);
}

void AnimationStateLayer::frame(float deltaTime)
{
	_stateMachine->frame(deltaTime);
}

void AnimationStateLayer::setWeight(float value)
{
	_weight = value;
}

float AnimationStateLayer::getWeight()
{
	return _weight;
}

float AnimationStateLayer::getPartialMaskWeight(float depth)
{
	if (_partialMaskRoot == nullptr)
		return 1.f;

	float result = depth - _partialMaskRoot->getDepth() + 1.f;

	if (result > _maskDepth)
		return 0.f;

	return MathEx::clamp01(result / _partialSmoothCount);
}

void AnimationStateLayer::setMaskBone(TransformStructure* bone)
{
	_partialMaskRoot = bone;
}

size_t AnimationStateLayer::getMaskBoneHashedName()
{
	return _partialMaskRoot == nullptr ? 0 : _partialMaskRoot->getHashedName();
}

TransformStructure* AnimationStateLayer::getMaskBone()
{
	return _partialMaskRoot;
}

void AnimationStateLayer::setMasking(bool value)
{
	_masking = value;
}

bool AnimationStateLayer::IsMasking()
{
	return _masking;
}

void AnimationStateLayer::setMaskDepth(float depth)
{
	_maskDepth = depth;
}

float AnimationStateLayer::getMaskDepth()
{
	return _maskDepth;
}

void AnimationStateLayer::setAdditive(bool value)
{
	_additive = value;
}

bool AnimationStateLayer::IsAdditive()
{
	return _additive;
}

void AnimationStateLayer::setSmoothCount(float value)
{
	_partialSmoothCount = value;
}

float AnimationStateLayer::getSmoothCount()
{
	return -_partialSmoothCount;
}

void AnimationStateLayer::setInversMasking(bool value)
{
	_inversMasking = value;
}

bool AnimationStateLayer::isInverseMasking()
{
	return _inversMasking;
}

void AnimationStateLayer::setAdditiveBasePose(AnimationDataPack* pose)
{
	_additiveBasePose = pose;
}

AnimationDataPack* AnimationStateLayer::getAdditiveBasePose()
{
	return _additiveBasePose;
}

Transform AnimationStateLayer::getAdditiveBasePose(size_t hashedName)
{
	return getAdditiveBasePose()->getPoseByIndex(0, hashedName);
}

//Transform AnimationStateLayer::getIncreasedTransform(TransformStructure* target)
//{
//	return _stateMachine->getIncreasedTransform(target);
//}

AnimationStateBase* AnimationStateLayer::findState(std::string name)
{
	return _stateMachine->findState(name);
}

std::string AnimationStateLayer::getName()
{
	return _name;
}

const std::vector<AnimationStateBase*>& AnimationStateLayer::getStates()
{
	return _stateMachine->getStates();
}

Transform AnimationStateLayer::getCurrentPose(TransformStructure* structure, size_t& outIndex)
{
	return _stateMachine->getCurrentPose(structure, outIndex);
}
