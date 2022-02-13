#pragma once
#include <string>
#include <unordered_map>

class Transform;
class TransformStructure;
class AnimationLayerBase
{
public:
									AnimationLayerBase();

	virtual void					createAnimationLayerBase(std::string name);
	virtual void					destroyAnimationLayerBase() = 0;

	virtual void					initialize() = 0;

	virtual void					frame(float deltaTime) = 0;

	std::string						getName();

	void							updateWorldCache(Transform local, TransformStructure* target);
	virtual Transform				getCurrentPose(TransformStructure* structure, size_t& outIndex, bool masking = false) = 0;
	virtual Transform				getCurrentWorldPose(TransformStructure* structure, bool& find);

	const std::unordered_map<size_t, Transform>& getPoseCache();
private:
	std::string						_name;
	std::unordered_map<size_t, Transform> _poseCahce;
};

