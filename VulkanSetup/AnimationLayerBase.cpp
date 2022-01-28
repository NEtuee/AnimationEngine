#include "AnimationLayerBase.h"
#include "TransformStructure.h"

AnimationLayerBase::AnimationLayerBase()
	:_name("")
{
}

void AnimationLayerBase::createAnimationLayerBase(std::string name)
{
	_name = name;
}

std::string AnimationLayerBase::getName()
{
	return _name;
}

void AnimationLayerBase::updateWorldCache(Transform local, TransformStructure* target)
{
	Transform cache = local;

	if (target->getParent() != nullptr)
	{
		auto iterator = _poseCahce.find(target->getParent()->getHashedName());

		if (iterator != _poseCahce.end())
		{
			cache = cache.localToWorld((*iterator).second);
		}
	}

	_poseCahce[target->getHashedName()] = cache;
}

Transform AnimationLayerBase::getCurrentWorldPose(TransformStructure* structure, bool& find)
{
	auto iterator = _poseCahce.find(structure->getHashedName());
	find = iterator != _poseCahce.end();
	if (find)
		return (*iterator).second;
	return Transform();
}

const std::unordered_map<size_t, Transform>& AnimationLayerBase::getPoseCache()
{
	return _poseCahce;
}
