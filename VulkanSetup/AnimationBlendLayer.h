#pragma once
#include <string>
#include <vector>
#include "AnimationLayerBase.h"

class Transform;
class TransformStructure;
class AnimationDataPack;
class AnimationBlendLayer : public AnimationLayerBase
{
public:
								AnimationBlendLayer();

	virtual void				destroyAnimationLayerBase() override;
	virtual void				initialize()				override;
	virtual void				frame(float deltaTime)		override;

	void						setWeight(float value);
	float						getWeight();
	float						getPartialMaskWeight(float depth);

	void						setMaskBone(TransformStructure* bone);
	size_t						getMaskBoneHashedName();
	TransformStructure*			getMaskBone();
	bool						isMaskBone(TransformStructure* bone);

	void						setMaskDepth(float depth);
	float						getMaskDepth();

	void						setAdditive(bool value);
	bool						IsAdditive();

	void						setSmoothCount(float value);
	float						getSmoothCount();

	void						setInversMasking(bool value);
	bool						isInverseMasking();

	void						setRotationRootBone(TransformStructure* root);
	void						setCharacterRoot(TransformStructure* root);

	void						setAdditiveBasePose(AnimationDataPack* pose);

	AnimationLayerBase*			getLayerOne();
	AnimationLayerBase*			getLayerTwo();

	void						setLayers(AnimationLayerBase* one, AnimationLayerBase* two);

	AnimationDataPack*			getAdditiveBasePose();
	Transform					getAdditiveBasePose(size_t hashedName);

	virtual Transform			getCurrentPose(TransformStructure* structure, size_t& outIndex, bool masking = false) override;
private:
	TransformStructure*			_partialMaskRoot;
	TransformStructure*			_rotationRoot;
	TransformStructure*			_characterRoot;

	AnimationDataPack*			_additiveBasePose;

	AnimationLayerBase*			_layer1;
	AnimationLayerBase*			_layer2;

	float						_weight;
	float						_maskDepth;
	float						_partialSmoothCount;
	bool						_additive;
};

