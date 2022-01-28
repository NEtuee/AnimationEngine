#include "AnimationBlendLayer.h"
#include "Transform.h"
#include "TransformStructure.h"
#include "AnimationDataPack.h"
#include "MathEx.h"

AnimationBlendLayer::AnimationBlendLayer()
	:_partialMaskRoot(nullptr), _additiveBasePose(nullptr), _layer1(nullptr), _layer2(nullptr),
	_weight(1.f),_maskDepth(0.f),_partialSmoothCount(0.f), _masking(false),_inverseMasking(false), _additive(false),
	_characterRoot(nullptr),_rotationRoot(nullptr)
{

}

void AnimationBlendLayer::destroyAnimationLayerBase()
{
	_layer1->destroyAnimationLayerBase();
	_layer2->destroyAnimationLayerBase();
}

void AnimationBlendLayer::initialize()
{
	_layer1->initialize();
	_layer2->initialize();
}

void AnimationBlendLayer::frame(float deltaTime)
{
	_layer1->frame(deltaTime);
	_layer2->frame(deltaTime);
}

void AnimationBlendLayer::afterLoop(TransformStructure* structure)
{
	_layer1->afterLoop(structure);
	_layer2->afterLoop(structure);

	if(getMaskBoneHashedName() == structure->getHashedName())
		setMasking(false);
}

void AnimationBlendLayer::setWeight(float value)
{
	_weight = value;
}

float AnimationBlendLayer::getWeight()
{
	return _weight;
}

float AnimationBlendLayer::getPartialMaskWeight(float depth)
{
	if (_partialMaskRoot == nullptr)
		return 1.f;

	float result = depth - _partialMaskRoot->getDepth() + 1.f;

	if (result > _maskDepth)
		return 0.f;

	return MathEx::clamp01(result / _partialSmoothCount);
}

void AnimationBlendLayer::setMaskBone(TransformStructure* bone)
{
	_partialMaskRoot = bone;
}

size_t AnimationBlendLayer::getMaskBoneHashedName()
{
	return _partialMaskRoot == nullptr ? 0 : _partialMaskRoot->getHashedName();
}

TransformStructure* AnimationBlendLayer::getMaskBone()
{
	return _partialMaskRoot;
}

void AnimationBlendLayer::setMasking(bool value)
{
	_masking = value;
}

bool AnimationBlendLayer::IsMasking()
{
	return _masking;
}

void AnimationBlendLayer::setMaskDepth(float depth)
{
	_maskDepth = depth;
}

float AnimationBlendLayer::getMaskDepth()
{
	return _maskDepth;
}

void AnimationBlendLayer::setAdditive(bool value)
{
	_additive = value;
}

bool AnimationBlendLayer::IsAdditive()
{
	return _additive;
}

void AnimationBlendLayer::setSmoothCount(float value)
{
	_partialSmoothCount = value;
}

float AnimationBlendLayer::getSmoothCount()
{
	return _partialSmoothCount;
}

void AnimationBlendLayer::setInversMasking(bool value)
{
	_inverseMasking = value;
}

bool AnimationBlendLayer::isInverseMasking()
{
	return _inverseMasking;
}

void AnimationBlendLayer::setRotationRootBone(TransformStructure* root)
{
	_rotationRoot = root;
}

void AnimationBlendLayer::setCharacterRoot(TransformStructure* root)
{
	_characterRoot = root;
}

void AnimationBlendLayer::setAdditiveBasePose(AnimationDataPack* pose)
{
	_additiveBasePose = pose;
}

AnimationLayerBase* AnimationBlendLayer::getLayerOne()
{
	return _layer1;
}

AnimationLayerBase* AnimationBlendLayer::getLayerTwo()
{
	return _layer2;
}

void AnimationBlendLayer::setLayers(AnimationLayerBase* one, AnimationLayerBase* two)
{
	_layer1 = one;
	_layer2 = two;
}

AnimationDataPack* AnimationBlendLayer::getAdditiveBasePose()
{
	return _additiveBasePose;
}

Transform AnimationBlendLayer::getAdditiveBasePose(size_t hashedName)
{
	return getAdditiveBasePose()->getPoseByIndex(0, hashedName);
}

Transform AnimationBlendLayer::getCurrentPose(TransformStructure* structure, int& outIndex)
{
	int twoIndex;
	Transform mainPose = _layer1->getCurrentPose(structure, outIndex);
	Transform blendTarget = _layer2->getCurrentPose(structure, twoIndex);

	updateWorldCache(mainPose, structure);

	if (getWeight() <= 0.f || twoIndex == -1)
		return mainPose;

	size_t hashedName = structure->getHashedName();

	if (hashedName == getMaskBoneHashedName())
	{
		setMasking(true);
	}
	else if (getMaskBone() != nullptr && IsMasking() == false)
		return mainPose;

	if (IsAdditive() && outIndex != -1)
	{
		blendTarget = blendTarget.sub(getAdditiveBasePose(hashedName));
		blendTarget = mainPose.add(blendTarget);
	}

	//mainPose = _layer2->getCurrentWorldPose(_partialMaskRoot, find);
	mainPose = mainPose.lerp(blendTarget, getWeight() * getPartialMaskWeight(structure->getDepth()));

	updateWorldCache(mainPose, structure);

	if (hashedName == getMaskBoneHashedName() && _rotationRoot != nullptr)
	{
		//bool find = false;
		//Transform world = _layer2->getCurrentWorldPose(_partialMaskRoot, find);

		//_partialMaskRoot->getParent()->updateWorld();
		//mainPose.setRotation(world.worldToLocal(_layer2->getCurrentWorldPose(structure->getParent(), find)).getRotation());
		//mainPose.setRotation(world.worldToLocal(_partialMaskRoot->getParent()->getWorldTransform()).getRotation());
		


		auto rootBone = _layer2->getCurrentPose(_rotationRoot, twoIndex);
		float angle = XMVectorGetX(XMVector3AngleBetweenNormals(_characterRoot->getLocalTransform().getForward(), rootBone.getForward())) - 3.141592f * 0.5f;
		XMVECTOR targetRotation = XMQuaternionMultiply(XMQuaternionRotationAxis(mainPose.getRight(), angle), mainPose.getRotation());
		targetRotation = XMQuaternionSlerp(mainPose.getRotation(), targetRotation, getWeight());
		mainPose.setRotation(targetRotation);


		//structure->SetLocalRotation(world.getRotation());
	}

	return mainPose;
}
