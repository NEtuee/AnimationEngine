#include "AnimationPlayer.h"
#include "AnimationDataPack.h"
#include "BoneStructure.h"
#include "TransformStructure.h"
#include "AnimationState.h"
#include "Transform.h"

//AnimationPlayer::AnimationPlayer()
//	:_animation(nullptr), _blendTarget(nullptr), _deltaTime(0.f), _prevTime(0.f), _prevBlendTime(0.f),_currentTime(0.f),
//	_blendTime(0.f), _blendDuration(0.f), _animationTime(0.f), _blending(false), _isEnd(false)
//{
//}
//
//void AnimationPlayer::initialize()
//{
//	_isEnd = false;
//	_currentTime = 0.f;
//	start();
//}
//
//void AnimationPlayer::createAnimationPlayer(AnimationState* animation)
//{
//	//setAnimation(animation);
//	initialize();
//}
//
//void AnimationPlayer::destroyAnimationPlayer()
//{
//	//delete _animation;
//}
//
////void AnimationPlayer::setAnimation(AnimationDataPack* animation)
////{
////	_animation = animation;
////	_animationTime = _animation->getTime();
////}
//
//void AnimationPlayer::playTimer(float deltaTime)
//{
//	deltaTime *= _animation->getSpeed();
//	_prevTime = _currentTime;
//	_deltaTime = deltaTime;
//
//	if (!isEnd())
//		_currentTime += deltaTime;
//	else
//		_isEnd = !_animation->isLoop();
//
//	if (_blending)
//	{
//		_prevBlendTime = _blendTime;
//		_blendTime += deltaTime;
//		if (_blendTime >= _blendDuration)
//		{
//			_currentTime = _blendTime;
//			_prevBlendTime = 0.f;
//
//			setAnimation(_blendTarget);
//
//			_blending = false;
//			_isEnd = false;
//		}
//	}
//
//	if (_currentTime >= _animationTime)
//	{
//		if (_animation->isLoop())
//		{
//			while (_currentTime > _animationTime)
//				_currentTime -= _animationTime;
//		}
//		else
//		{
//			_currentTime = _animationTime;
//			_isEnd = true;
//		}
//	}
//}
//
//void AnimationPlayer::start()
//{
//	_isEnd = false;
//	_blending = false;
//	_currentTime = 0.f;
//	_blendTime = 0.f;
//}
//
//void AnimationPlayer::startBlend(AnimationDataPack* target, float duration)
//{
//	_blending = true;
//	_blendTarget = target;
//	_blendTime = 0.f;
//	_blendDuration = duration;
//}
//
//void AnimationPlayer::play(float deltaTime, BoneStructure* bone)
//{
//	playTimer(deltaTime);
//	setCurrentPoseToBoneBySecond(_currentTime, bone);
//}
//
//void AnimationPlayer::setLoop(bool loop)
//{
//	_animation->setLoop(loop);
//}
//
//void AnimationPlayer::setTime(float t)
//{
//	_currentTime = t;
//}
//
//void AnimationPlayer::setCurrentPoseToBoneBySecond(float time, BoneStructure* bone)
//{
//	setcurrentPoseToBone(&bone->getRootBone());
//	bone->getRootBone().updateWorld();
//}
//
//float AnimationPlayer::getCurrentTime()
//{
//	return _currentTime;
//}
//
//float AnimationPlayer::getAnimationTime()
//{
//	return _animationTime;
//}
//
//float AnimationPlayer::getAnimationPercentage()
//{
//	return _currentTime / _animationTime;
//}
//
//bool AnimationPlayer::isBlending()
//{
//	return _blending;
//}
//
//bool AnimationPlayer::isLoop()
//{
//	return _animation->isLoop();
//}
//
//bool AnimationPlayer::isEnd()
//{
//	return _isEnd;
//}
//
//Transform AnimationPlayer::getIncreasePerFrameReal(TransformStructure* structure, size_t& outIndex)
//{
//	Transform current = getIncreasePerFrame(_animation, _currentTime, _prevTime, structure, outIndex);
//	if (_blending)
//	{
//		Transform blend = getIncreasePerFrame(_blendTarget, _blendTime, _prevBlendTime, structure, outIndex);
//		current = current.lerp(blend, _blendTime / _blendDuration);
//	}
//
//
//	return current;
//}
//
//Transform AnimationPlayer::getIncreasePerFrame(AnimationDataPack* data, float currentTime, float prevTime, TransformStructure* structure, size_t& outIndex)
//{
//	Transform current = data->getPoseByTime(outIndex, currentTime, structure->getHashedName());
//	Transform increase = data->getPoseByTime(outIndex, prevTime, structure->getHashedName());
//
//	if (data->isLoop() && prevTime > currentTime)
//	{
//		increase = data->getPoseByTime(outIndex, _animationTime, structure->getHashedName()).sub(increase);
//		increase = increase.add(current.sub(data->getPoseByTime(outIndex, 0.f, structure->getHashedName())));
//	}
//	else
//		increase = current.sub(increase);
//
//
//	return increase;
//}
//
//Transform AnimationPlayer::getcurrentPose(TransformStructure* structure, size_t& outIndex)
//{
//	Transform pose;
//
//	if (!_blending)
//		pose = _animation->getPoseByTime(outIndex, _currentTime, structure->getHashedName());
//	else
//	{
//		int blendIndex = 0;
//		Transform blendTarget = _blendTarget->getPoseByTime(blendIndex, _blendTime, structure->getHashedName());
//
//		if (blendIndex == -1)
//		{
//			//pose = _animation->getPoseByTime(outIndex, _currentTime, structure->getHashedName());
//			blendTarget = structure->getLocalTransform();
//		}
//
//		pose = _animation->getBlendPoseByTime(outIndex, _currentTime, _blendTime / _blendDuration, structure, blendTarget);
//
//
//	}
//
//	return pose;
//}
//
//std::string AnimationPlayer::getBlendTargetName()
//{
//	if (_blending)
//		return _blendTarget->getName();
//	else
//		return "";
//}
//
//std::string AnimationPlayer::getName()
//{
//	return _animation->getName();
//}
//
//void AnimationPlayer::setcurrentPoseToBone(TransformStructure* root)
//{
//	int index = 0;
//	size_t hashedName = root->getHashedName();
//	Transform pose = getcurrentPose(root, index);
//
//	if(index != -1)
//		root->SetLoaclTransformNoCalc(pose.getPosition(), pose.getScale(), pose.getRotation());
//
//	auto children = root->getChildren();
//	for (auto iterator = children.begin(); iterator != children.end(); ++iterator)
//	{
//		setcurrentPoseToBone((*iterator));
//	}
//}
//
