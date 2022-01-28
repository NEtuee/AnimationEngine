#pragma once
#include <string>
#include <unordered_map>

struct BoneDataRow;

class AnimationState;
class TransformStructure;
class Transform;
class BoneStructure;

class AnimationPlayer
{
//public:
//						AnimationPlayer();
//
//	void				initialize();
//	void				start();
//	void				startBlend(AnimationState* target, float duration);
//	void				createAnimationPlayer(AnimationState* animation);
//	void				destroyAnimationPlayer();
//	
//
//	//void				setAnimation(AnimationDataPack* animation);
//	void				playTimer(float deltaTime);
//	void				play(float deltaTime, BoneStructure* bone);
//	void				setLoop(bool loop);
//
//	void				setTime(float t);
//	void				setCurrentPoseToBoneBySecond(float time, BoneStructure* bone);
//
//	float				getCurrentTime();
//	float				getAnimationTime();
//	float				getAnimationPercentage();
//
//	bool				isBlending();
//	bool				isLoop();
//	bool				isEnd();
//
//	/*Transform			getIncreasePerFrameReal(TransformStructure* structure,int& outIndex);
//	Transform			getIncreasePerFrame(AnimationDataPack* data, float currentTime, float prevTime, TransformStructure* structure, int& outIndex);*/
//	Transform			getcurrentPose(TransformStructure* structure,int& outIndex);
//	std::string			getBlendTargetName();
//	std::string			getName();
//	//AnimationDataPack*	getAnimation()			{ return _animation; }
//private:
//	void				setcurrentPoseToBone(TransformStructure* root);
//private:
//	AnimationState*	_animation;
//	AnimationState*	_blendTarget;
//
//	float				_deltaTime;
//	float				_prevTime;
//	float				_prevBlendTime;
//	float				_currentTime;
//	float				_blendTime;
//	float				_blendDuration;
//	float				_animationTime;
//
//	bool				_blending;
//	bool				_isEnd;
};

