#include "AnimationDataPack.h"
#include "AnimationDataRow.h"
#include "TransformStructure.h"
#include "Transform.h"

AnimationDataPack::AnimationDataPack()
	:_name(""), _animation(nullptr), _speed(0.f), _time(0.f), _frameSecond(0.f), _fps(0), _frameCount(0), _isLoop(false)
{

}

void AnimationDataPack::createAnimationDataPack(AnimationDataRow* rowData, std::string name)
{
	_animation = rowData;
	setName(name);
	initialize();
}

void AnimationDataPack::createAnimationDataPack(TransformStructure* transform, std::string name)
{
	AnimationDataRow* row = new AnimationDataRow;
	row->_boneScale = 1.f;
	row->_boneConut = transform->getChildCountAll() + 1;
	row->_fps = 1;
	row->_frameCount = 1;
	row->_isAdditive = false;
	row->_isFacial = false;
	row->_isSinglebone = false;

	createAnimationDataPackFromTransforms(transform, row);

	_animation = row;
	setName(name);
	initialize();
}

void AnimationDataPack::destroyAnimationDataPack()
{
	delete _animation;
}

void AnimationDataPack::initialize()
{
	_fps = _animation->_fps;
	_frameCount = _animation->_frameCount;
	_time = static_cast<float>(_frameCount) / static_cast<float>(_fps);
	_frameSecond = 1.f / static_cast<float>(_fps);
	_speed = 1.f;
	_isLoop = false;
}

Transform AnimationDataPack::getPoseByPercentage(int& outIndex, float percentage, size_t hashedName)
{
	float time = _time * percentage;

	return getPoseByTime(outIndex, time, hashedName);
}

Transform AnimationDataPack::getPoseByTime(int& outIndex, float time, size_t hashedName)
{
	if (isLoop())
		time = std::fmod(time, getTime());

	auto bone = findBone(hashedName);
	if (bone == nullptr)
	{
		outIndex = -1;
		return Transform();
	}

	int startIndex = timeToStartIndex(time);
	outIndex = findNearestIndex(startIndex, bone);

	auto& vec = bone->_frames;

	Transform pose;

	if (outIndex == -1)
	{
	}
	else if (outIndex >= vec.size() - 1)
	{
		auto& frame = vec[outIndex];
		pose.setTransform(frame.getPositionVector(), frame.getScaleVector(), frame.getRotationVector());
			
	}
	else
	{
		int current = vec[outIndex].frame;
		int next = vec[outIndex + 1].frame;

		float currTime = indexToTime(current);
		float nextTime = indexToTime(next);

		float factor = (time - currTime) / (nextTime - currTime);

		pose.setTransform(
			XMVectorLerp(vec[outIndex].getPositionVector(), vec[outIndex + 1].getPositionVector(), factor),
			XMVectorLerp(vec[outIndex].getScaleVector(), vec[outIndex + 1].getScaleVector(), factor),
			XMQuaternionSlerp(vec[outIndex].getRotationVector(), vec[outIndex + 1].getRotationVector(), factor));
	}


	return pose;
}

Transform AnimationDataPack::getPoseByIndex(int index, size_t hashedName)
{
	auto bone = findBone(hashedName);
	if (bone == nullptr)
	{
		return Transform();
	}

	auto& vec = bone->_frames;

	if (index < 0 || index > vec.size())
		return Transform();

	Transform pose;
	pose.setTransform(vec[index].getPositionVector(), vec[index].getScaleVector(), vec[index].getRotationVector());

	return pose;
}

Transform AnimationDataPack::getBlendPoseByTime(int& outIndex, float time, float factor, TransformStructure* structure, const Transform& target)
{
	Transform pose = getPoseByTime(outIndex, time, structure->getHashedName());
	if (outIndex == -1)
	{
		pose = structure->getLocalTransform();
		outIndex = 0;
	}

	pose = pose.lerp(target, factor);
	return pose;
}

BoneDataRow* AnimationDataPack::findBone(size_t hashedName)
{
	auto iterator = (_animation->_bones.find(hashedName));
	return (iterator == _animation->_bones.end()) ? nullptr : &(*iterator).second;
}

void AnimationDataPack::createAnimationDataPackFromTransforms(TransformStructure* transform, AnimationDataRow* row)
{
	FrameData frame;
	frame.frame = 0;
	frame.position.StoreVector(transform->getLocalPosition());
	frame.scale.StoreVector(transform->getLocalScale());
	frame.rotation.StoreVector(transform->getLocalRotation());

	//XMStoreFloat3(&frame.position, transform->getLocalPosition());
	//XMStoreFloat3(&frame.scale, transform->getLocalScale());
	//XMStoreFloat4(&frame.rotation, transform->getLocalRotation());

	BoneDataRow bone;
	bone._frames.push_back(frame);
	bone._name = transform->getName();
	row->_bones.emplace(transform->getHashedName(), bone);

	auto& children = transform->getChildren();
	for (auto iterator = children.begin(); iterator != children.end(); ++iterator)
	{
		createAnimationDataPackFromTransforms((*iterator), row);
	}
}

int AnimationDataPack::timeToStartIndex(float time)
{
	int index = static_cast<int>(time / _frameSecond);
	index = index >= _frameCount ? _frameCount - 1 : index;
	return index;
}

int AnimationDataPack::findNearestIndex(int startIndex, const BoneDataRow* bone)
{
	auto iterator = bone->_frames.begin();
	for (; iterator != bone->_frames.end(); ++iterator)
	{
		if ((*iterator).frame == startIndex)
		{
			return static_cast<int>(iterator - bone->_frames.begin());
		}
		else if ((*iterator).frame > startIndex)
		{
			return static_cast<int>(iterator - bone->_frames.begin()) - 1;
		}
	}

	if (bone->_frames.size() == 0)
		return -1;

	return bone->_frames[bone->_frames.size() - 1].frame;
}

float AnimationDataPack::indexToTime(int index)
{
	return static_cast<float>(index) * _frameSecond;
}
