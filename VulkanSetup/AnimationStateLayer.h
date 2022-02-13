#pragma once
#include <string>
#include <vector>


class Transform;

class AnimationStateMachine;
class AnimationStateBase;
class AnimationDataPack;
class BoneStructure;
class TransformStructure;
class AnimationStateLayer
{
public:
											AnimationStateLayer();

	void									createAnimationStateLayer(std::string name, float weight);
	void									destroyAnimationStateLayer();

	void									initialize();

	void									addState(AnimationStateBase* state);
	void									deleteState(size_t index);

	void									setDefaultState(AnimationStateBase* state);
	void									setDefaultState(std::string name);

	void									frame(float deltaTime);

	void									setWeight(float value);
	float									getWeight();
	float									getPartialMaskWeight(float depth);

	void									setMaskBone(TransformStructure* bone);
	size_t									getMaskBoneHashedName();
	TransformStructure*						getMaskBone();
	void									setMasking(bool value);
	bool									IsMasking();
	void									setMaskDepth(float depth);
	float									getMaskDepth();
	void									setAdditive(bool value);
	bool									IsAdditive();
	void									setSmoothCount(float value);
	float									getSmoothCount();
	void									setInversMasking(bool value);
	bool									isInverseMasking();

	void									setAdditiveBasePose(AnimationDataPack* pose);
	AnimationDataPack*						getAdditiveBasePose();
	Transform								getAdditiveBasePose(size_t hashedName);
	//Transform								getIncreasedTransform(TransformStructure* target);

	AnimationStateBase*						findState(std::string name);

	std::string								getName();
	const std::vector<AnimationStateBase*>&	getStates();
	Transform								getCurrentPose(TransformStructure* structure, size_t& outIndex);
private:
	AnimationStateMachine*					_stateMachine;
	std::string								_name;
	TransformStructure*						_partialMaskRoot;
	AnimationDataPack*						_additiveBasePose;

	float									_weight;
	float									_maskDepth;
	float									_partialSmoothCount;
	bool									_masking;
	bool									_inversMasking;
	bool									_additive;
	
};

