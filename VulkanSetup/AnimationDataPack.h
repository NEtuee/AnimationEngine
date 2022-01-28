#pragma once
#include <string>

struct AnimationDataRow;
struct BoneDataRow;

class Transform;
class TransformStructure;

class AnimationDataPack
{
public:
							AnimationDataPack();

	void					createAnimationDataPack(AnimationDataRow* rowData, std::string name);
	void					createAnimationDataPack(TransformStructure* transform, std::string name);
	void					destroyAnimationDataPack();
	void					initialize();

	Transform				getPoseByPercentage(int& outIndex, float percentage, size_t hashedName);
	Transform				getPoseByTime(int& outIndex, float time, size_t hashedName);
	Transform				getPoseByIndex(int index, size_t hashedName);
	Transform				getBlendPoseByTime(int& outIndex, float time, float factor, TransformStructure* structure, const Transform& target);

	BoneDataRow*			findBone(size_t hashedName);

	inline void				setName(std::string name)	{ _name = name; }
	inline void				setLoop(bool value)			{ _isLoop = value; }
	inline void				setSpeed(float value)		{ _speed = value; }

	inline std::string		getName()					{ return _name; }
	inline bool				isLoop()					{ return _isLoop; }
	inline float			getSpeed()					{ return _speed; }
	inline int				getFps()					{ return _fps; }
	inline int				getFrameCount()				{ return _frameCount; }
	inline float			getTime()					{ return _time; }
	inline float			getFrameSecond()			{ return _frameSecond; }

	inline AnimationDataRow* getRow()					{ return _animation; }
private:
	void					createAnimationDataPackFromTransforms(TransformStructure* transform, AnimationDataRow* row);
	int						timeToStartIndex(float time);
	int						findNearestIndex(int startIndex, const BoneDataRow* bone);
	float					indexToTime(int index);
private:
	std::string				_name;

	AnimationDataRow*		_animation;
	float					_speed;
	float					_time;
	float					_frameSecond;
	int						_fps;
	int						_frameCount;

	bool					_isLoop;
	
};

